// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUPlayerState.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    void SetTeamID(const int32 ID) { TeamID = ID; }
    int32 GetTeamID() { return TeamID; }

    void SetTeamColor(const FLinearColor Color) { TeamColor = Color; }
    FLinearColor GetTeamColor() { return TeamColor; }

    void AddKill() { ++KillsNum; }
    int32 GetKillsNum() { return KillsNum; }

    void AddDeath() { --DeathNum; }
    int32 GetDeathNum() { return DeathNum; }

    void logInfo();

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathNum = 0;
};
