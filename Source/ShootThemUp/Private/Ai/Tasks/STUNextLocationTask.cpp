// Shoot Them Uo Game. All Right Reserved

#include "Ai/Tasks/STUNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

USTUNextLocationTask::USTUNextLocationTask()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavSys) return EBTNodeResult::Failed;

    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();

    if (!SelfCenter)
    {
        auto CentrActor = Cast<AActor>(Blackboard->GetValueAsObject(CentrActorKey.SelectedKeyName));
        if (!CentrActor) return EBTNodeResult::Failed;
        UE_LOG(LogTemp, Warning, TEXT("%s"), *CentrActor->GetName());
        Location = CentrActor->GetActorLocation();
    }

    const auto Foudn = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!Foudn) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    return EBTNodeResult::Succeeded;
}
