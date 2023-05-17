#pragma once
#include "Player/STUPlayerState.h"

class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());

        return Cast<T>(Component);
    }

    bool static AreEnemeis(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

        const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);
        if (!PlayerState1 || !PlayerState2) return false;

        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }

    FText static TextFromInt(const int32 value) { return FText::FromString(FString::FromInt(value)); }
};