// Shoot Them Uo Game. All Right Reserved

#include "Ai/STUAiCharacter.h"
#include "Ai/STUAIController.h"

ASTUAiCharacter::ASTUAiCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass;
}