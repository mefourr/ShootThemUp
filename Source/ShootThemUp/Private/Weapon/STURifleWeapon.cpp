// Shoot Them Uo Game. All Right Reserved

#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Weapon/Components/STUWeaponFXComponent.h"


ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    Super::StartFire();

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimerBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}
    
void ASTURifleWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceEnd, TraceStart;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        //нахождение направлениея конечной точки трейса
        const FVector HitDirection = (HitResult.ImpactPoint - GetMuzzleTranform().GetLocation()).GetSafeNormal();

        const float Degrees =
            FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetMuzzleTranform().GetRotation().GetForwardVector(), HitDirection)));

        if (Degrees > 90.0f)
        {
            DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), TraceEnd, FColor::Green, false, 2.0f, 0, 3.0f);
            return;
        }

        MakeDamage(HitResult);

        WeaponFXComponent->PlayImpactFX(HitResult);

        //DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 3.0f);
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Red, false, 2.0f, 0, 1.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), TraceEnd, FColor::Green, false, 2.0f, 0, 3.0f);
    }

    DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;  // SocketTransform.GetLocation();

    const auto HalfRad = FMath::DegreesToRadians(BuletSpread);

    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);  // SocketTransform.GetRotation().GetForwardVector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistace;

    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}