// Shoot Them Uo Game. All Right Reserved 


#include "Weapon/STUProjectile.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTUProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

