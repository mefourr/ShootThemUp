// Shoot Them Uo Game. All Right Reserved

#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    if (!GetWorld()) return;

    // GetOwner() - return ��������� �� ��������� ����������: ASTUBaseCharacter.
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character) return;

    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    CurrentWeapon->SetOwner(Character);
}

void USTUWeaponComponent::StartFire()
{

    if (!CurrentWeapon) return;

    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->StopFire();
}