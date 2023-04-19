// Shoot Them Uo Game. All Right Reserved

#include "Ai/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "Components/STUWeaponComponent.h"

USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;

    const auto WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>();
    if (!WeaponComponent) return false;

    return WeaponComponent->NeedAmmo(WeaponType);
}
