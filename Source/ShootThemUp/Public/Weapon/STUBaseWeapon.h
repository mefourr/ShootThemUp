// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "STUCoreTypes.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
    bool CanReload() const;

    bool IsFiring() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistace = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{30, 10, false};

    virtual void BeginPlay() override;
    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    AController* GetPlayerController() const;
    FTransform GetMuzzleTranform() const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    void LogAmmo();


private:
    FAmmoData CurrentAmmo;

    bool FireInProgress = false;
};
