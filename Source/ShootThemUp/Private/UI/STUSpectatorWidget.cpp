// Shoot Them Uo Game. All Right Reserved

#include "UI/STUSpectatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"

bool USTUSpectatorWidget::GetRespawnTIme(int32& CountDownTimer) const
{
    const auto RespawnComponent = GetOwningPlayer()->FindComponentByClass<USTURespawnComponent>();
    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

    CountDownTimer = RespawnComponent->GetRespawnCountDown();
    return true;
}