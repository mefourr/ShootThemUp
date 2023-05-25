// Shoot Them Uo Game. All Right Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
    FireInProgress = true;
}

void ASTUBaseWeapon::StopFire()
{
    FireInProgress = false;
}

bool ASTUBaseWeapon::IsFiring() const
{
    return FireInProgress;
}

void ASTUBaseWeapon::MakeShot() {}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if (!STUCharacter) return false;

    if (STUCharacter->IsPlayerControlled())
    {
        const auto Controller = STUCharacter->GetController<APlayerController>();
        if (!Controller) return false;

        Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    }
    else
    {
        ViewLocation = GetMuzzleTranform().GetLocation();
        ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }
    return true;
}

// потом возможно наюо переделать
FTransform ASTUBaseWeapon::GetMuzzleTranform() const
{
    return WeaponMesh->GetSocketTransform(MuzzleSocketName);  // WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;

    const FVector ShootDirection = ViewRotation.Vector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistace;

    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    // массив аккторов которых игнорирует трейс
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

    // фенкция из которой мы получаем данные о пересечении трейса с первым объектом
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Clip is empty"))
        return;
    }

    CurrentAmmo.Bullets--;
    // LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Display, TEXT("No more Clips"))
            return;
        }

        CurrentAmmo.Clips--;
    }

    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips) + " / ";
    AmmoInfo += IsAmmoEmpty() ? "IsAmmoEmpty: true" : "IsAmmoEmpty; false";

    //   UE_LOG(LogBaseWeapon, Warning, TEXT("%s"), *AmmoInfo);
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Bullets == DefaultAmmo.Bullets &&  //
           CurrentAmmo.Clips == DefaultAmmo.Clips;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount + 1, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
        // UE_LOG(LogBaseWeapon, Warning, TEXT("Ammo was empty %i"), ClipsAmount);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextCipsAmount = CurrentAmmo.Clips + ClipsAmount;

        if (CurrentAmmo.Clips - NextCipsAmount >= 0)
        {
            // UE_LOG(LogBaseWeapon, Warning, TEXT("Clips were added"));

            CurrentAmmo.Clips = NextCipsAmount;
        }
        else
        {
            //  UE_LOG(LogBaseWeapon, Warning, TEXT("Ammo is full now"));

            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        }
    }
    else
    {
        //  UE_LOG(LogBaseWeapon, Warning, TEXT("Bullets were added"));
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    }
    return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(  //
        MuzzleFX,                                         //
        WeaponMesh,                                       //
        MuzzleSocketName,                                 //
        FVector::ZeroVector,                              //
        FRotator::ZeroRotator,                            //
        EAttachLocation::SnapToTarget,                    //
        true);
}