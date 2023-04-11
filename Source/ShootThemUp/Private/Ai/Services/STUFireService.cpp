// Shoot Them Uo Game. All Right Reserved

#include "Ai/Services/STUFireService.h"
#include "Components/STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto BlackBoard = OwnerComp.GetBlackboardComponent();

    const auto HisAim = BlackBoard && BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
        if (!WeaponComponent) return;

        HisAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
    }



    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}