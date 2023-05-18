// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUGoToMainWidget.generated.h"

class UButton;

UCLASS()
class SHOOTTHEMUP_API USTUGoToMainWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UButton* BackToMainMenu;

private:
    UFUNCTION()
    void OnBackToMainMenu();
};
