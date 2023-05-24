// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUCoreTypes.h"
#include "STUMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTUGameInstance;
class USTULevelItemWidget;

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* HideAnimation;

    UPROPERTY(meta = (BindWidget))
    UButton* StartGame;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGame;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

private:
    UPROPERTY()
    TArray<USTULevelItemWidget*> LevelItemWidgets;

    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();

    void InitLevelItems();
    void OnLevelSelected(const FLevelData& Data);
    USTUGameInstance* GetSTUGameInstance() const;
};
