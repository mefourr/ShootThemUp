// Shoot Them Uo Game. All Right Reserved 


#include "Ai/STUAIController.h"
#include "Ai/STUAICharacter.h"

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    const auto STUAICharacter = Cast<ASTUAiCharacter>(InPawn);
    if (STUAICharacter)
    {
        RunBehaviorTree(STUAICharacter->BehaviorTreeAsset);
    }

}