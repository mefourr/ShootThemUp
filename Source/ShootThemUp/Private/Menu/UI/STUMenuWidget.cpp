// Shoot Them Uo Game. All Right Reserved

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGame)
    {
        StartGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }
}

void USTUMenuWidget::OnStartGame()
{
    const FName StartUpLevelName = "TestMap";
    UGameplayStatics::OpenLevel(this, StartUpLevelName);
}
