// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;
class UPointLightComponent;
class UProjectileMovementComponent;
class USTUWeaponFXComponent;

class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

    void SetShootDirection(const FVector& Direction) { ShootDirection = Direction; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
    UAudioComponent* AudioComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UNiagaraComponent* NiagaraComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UStaticMeshComponent* Sphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    UPointLightComponent* PointLightComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    USphereComponent* CollisionComponent;

    // зименяет положение актора на тик, в зависимости настроенных в нем параметров.
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeSeconds = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    USTUWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;

private:
    FVector ShootDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    AController* GetController() const;

    void DestroyRocket();
};