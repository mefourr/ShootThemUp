// Shoot Them Uo Game. All Right Reserved

#include "Ai/Decorators/STUHealthPercentDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

USTUHealthPercentDecorator::USTUHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool USTUHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto HaelthComponent = Controller->GetPawn()->FindComponentByClass<USTUHealthComponent>();
    if (!HaelthComponent || HaelthComponent->IsDead()) return false;

    return HaelthComponent->GetHealthPercent() <= HealthPercent;
}
