// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()
public:
    FLevelData GetStartUpLevel() const { return StartUpLevel; }
    void SetStartUpLevel(FLevelData Data) { StartUpLevel = Data; }

    TArray<FLevelData> GetLevelData() const { return LevelIsData; }
    FName GetMainLevelName() const { return MainLevelName; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level name must be unique!"))
    TArray<FLevelData> LevelIsData;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MainLevelName = NAME_None;

private:
    FLevelData StartUpLevel;
};
