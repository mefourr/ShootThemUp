// Shoot Them Uo Game. All Right Reserved

#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaswPickup, All, All)

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);

    GenerateRotationYaw();
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));

    //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, FString::Printf(TEXT("RotationYaw: %f"), RotationYaw));
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::PickupWasTaken()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    if (GetRootComponent())
	{
        GetRootComponent()->SetVisibility(false, true);
	}

    FTimerHandle RespawnTimerHandle;
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTimer);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();

    CollisionComponent->SetCollisionResponseToAllChannels((ECollisionResponse::ECR_Overlap));

    if (GetRootComponent())
	{
        GetRootComponent()->SetVisibility(true, true);
	}
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}