// Shoot Them Uo Game. All Right Reserved 


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaswPickup, All, All)


ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;


}


void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

