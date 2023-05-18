// Shoot Them Uo Game. All Right Reserved

#include "Menu/STUMenuPlayerController.h"
#include "Menu/STUMenuGameModeBase.h"
#include "Menu/UI/STUMenuHUD.h"
#include "STUGameInstance.h"

void ASTUMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetInputMode(FInputModeUIOnly());
    bShowMouseCursor = true;
}