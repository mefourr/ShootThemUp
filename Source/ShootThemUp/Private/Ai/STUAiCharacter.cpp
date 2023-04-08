// Shoot Them Uo Game. All Right Reserved

#include "Ai/STUAiCharacter.h"
#include "Ai/STUAIController.h"
#include "GameFrameWork/CharacterMovementComponent.h"

ASTUAiCharacter::ASTUAiCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}