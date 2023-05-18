// Shoot Them Uo Game. All Right Reserved

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGame)
    {
        StartGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }

    if (QuitGame)
    {
        QuitGame->OnClicked.AddDynamic(this, &USTUMenuWidget::OnQuitGame);
    }
}

void USTUMenuWidget::OnStartGame()
{
    if (!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance) return;

    if (STUGameInstance->GetStartUpLevelName().IsNone())
    {
        UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartUpLevelName());
}

void USTUMenuWidget::OnQuitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}