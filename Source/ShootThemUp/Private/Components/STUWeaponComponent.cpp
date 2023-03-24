// Shoot Them Uo Game. All Right Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "Player/STUBaseCharacter.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only [%i] weapon items"), WeaponNum);

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

        Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnClipEmpty);
        
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

    const auto CurrentWeaponData = WeaponData.FindByPredicate(      //
        [&](const FWeaponData& Data) {                              //
            return Data.WeaponClasse == CurrentWeapon->GetClass();  //
        }                                                           //
    );

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
    auto EquipFinishedAnimNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedAnimNotify)
    {
        EquipFinishedAnimNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgeten to set"))
        checkNoEntry();
    }

    for (auto& OnWeaponData : WeaponData)
    {
        auto ReloadFinishedAnimNotify = AnimUtils::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OnWeaponData.ReloadAnimMantage);
        if (!ReloadFinishedAnimNotify)
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgeten to set"))
            checkNoEntry();
        }
        ReloadFinishedAnimNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character || Character->GetMesh() != MeshComp) return;

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character || Character->GetMesh() != MeshComp) return;
    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire()
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip()
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload()
{
    return CurrentWeapon             //
           && !EquipAnimInProgress   //
           && !ReloadAnimInProgress  //
           && CurrentWeapon->CanReload();
}

void USTUWeaponComponent::Reload()
{
    ChangeClip();
}

void USTUWeaponComponent::OnClipEmpty(ASTUBaseWeapon* AmmoEmptyWeapon)
{
    if (!AmmoEmptyWeapon) return;

    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        ChangeClip();
    }
    else
    {
        for (auto& Weapon : Weapons)
        {
            if (Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
            }
        }
    }
}

void USTUWeaponComponent::ChangeClip()
{
    if (!CanReload()) return;

    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();

    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::IsFiring() const
{
    return CurrentWeapon && CurrentWeapon->IsFiring();
}

bool USTUWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}

 bool USTUWeaponComponent::GetAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}

bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for (const auto& Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
    }
    return false;
}
