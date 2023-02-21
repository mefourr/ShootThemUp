// Shoot Them Uo Game. All Right Reserved

#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "Gameframework/ProjectileMovementComponent.h"

ASTUProjectile::ASTUProjectile()
{

    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(20.0f);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 3000.0f;
    MovementComponent->ProjectileGravityScale = 1.0f;
}

void ASTUProjectile::BeginPlay()
{
    Super::BeginPlay();
    
    check(MovementComponent);
    MovementComponent->Velocity = ShootDirection * MovementComponent->InitialSpeed;
    SetLifeSpan(5.0f);
}
 