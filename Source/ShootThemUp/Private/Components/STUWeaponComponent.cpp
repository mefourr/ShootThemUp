// Shoot Them Uo Game. All Right Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Animations/STUEquipFinishedAnimNotify.h"

#include "Player/STUBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;

    for (auto& EWeapon : Weapons)
    {
        EWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        EWeapon->Destroy();
    }

    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character || !GetWorld()) return;

    for (auto& OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClasse);

        if (!Weapon) continue;

        Weapon->SetOwner(Character);

        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("Invalid weapon index"))
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMantage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //
        return Data.WeaponClasse == CurrentWeapon->GetClass();                                //
    });

    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMantage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;

    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimations()
{
    if (!EquipAnimMontage) return;

    const auto NotifyEvents = EquipAnimMontage->Notifies;

    for (auto& NotifyEvent : NotifyEvents)
    {
        auto EquipFinishedNotify = Cast<USTUEquipFinishedAnimNotify>(NotifyEvent.Notify);

        if (!EquipFinishedNotify) return;

        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);

        break;
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character || Character->GetMesh() != MeshComp) return;

    EquipAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire()
{
    return CurrentWeapon && !EquipAnimInProgress;
}

bool USTUWeaponComponent::CanEquip()
{
    return !EquipAnimInProgress;
}

void USTUWeaponComponent::Reload()
{
    PlayAnimMontage(CurrentReloadAnimMontage);
}