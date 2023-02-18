// Shoot Them Uo Game. All Right Reserved

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

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
}

void ASTUBaseWeapon::Fire()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("BANGH!")), true, FVector2D(1.2f, 1.2f));

    MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld()) return;

    FVector TraceEnd, TraceStart;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        //назождение направлениея конечной точки трейса
        const FVector HitForward = (HitResult.ImpactPoint - GetMuzzleTranform().GetLocation()).GetSafeNormal();

        const float Degrees =
            FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(GetMuzzleTranform().GetRotation().GetForwardVector(), HitForward)));
        if (Degrees > 90.0f)
        {
            DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), TraceEnd, FColor::Green, false, 2.0f, 0, 3.0f);
            return;
        }

        MakeDamage(HitResult);

        DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 2.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 32, FColor::Red, false, 2.0f, 0, 1.0f);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleTranform().GetLocation(), TraceEnd, FColor::Green, false, 2.0f, 0, 3.0f);
    }
}

AController* ASTUBaseWeapon::GetPlayerController() const
{
    // каст классу ACharacter указателя из фунции GetOwner()
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
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

    TraceStart = ViewLocation;                             // SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector();  // SocketTransform.GetRotation().GetForwardVector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistace;

    return true;
}

void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    // массив аккторов которых игнорирует трейс
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    // фенкция из которой мы получаем данные о пересечении трейса с первым объектом
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(const FHitResult HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;

    DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}