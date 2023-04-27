// Shoot Them Uo Game. All Right Reserved

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PerceiveActors;

    // функция заполнит массив если кого-то увидит.
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
    if (PerceiveActors.Num() == 0) return nullptr;

    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto& Actor : PerceiveActors)
    {
        const auto HealthComponent = Actor->FindComponentByClass<USTUHealthComponent>();

        const auto PerceivePawn = Cast<APawn>(Actor);
        const auto AreEnemies = PerceivePawn && STUUtils::AreEnemeis(Controller, PerceivePawn->Controller);

        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = Actor;
            }
        }
    }

    return BestPawn;
}