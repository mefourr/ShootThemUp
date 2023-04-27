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

private:
    int32 TeamID;
    FLinearColor TeamColor;
};
