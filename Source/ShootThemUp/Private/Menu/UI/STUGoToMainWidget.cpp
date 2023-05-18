// Shoot Them Uo Game. All Right Reserved

#include "Menu/UI/STUGoToMainWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGoToMainWidget, All, All);

void USTUGoToMainWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (BackToMainMenu)
    {
        BackToMainMenu->OnClicked.AddDynamic(this, &USTUGoToMainWidget::OnBackToMainMenu);
    }
}

void USTUGoToMainWidget::OnBackToMainMenu()
{
    if (!GetWorld()) return;

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance) return;

    if (STUGameInstance->GetMainLevelName().IsNone())
    {
        UE_LOG(LogSTUGoToMainWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetMainLevelName());
}
