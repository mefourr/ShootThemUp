// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUGameInstance.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    FString TestString = "Hello Ghame";

    FName GetStartUpLevelName() const { return StartUpLevelName; }
    FName GetMainLevelName() const { return MainLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName StartUpLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MainLevelName = NAME_None;
};
