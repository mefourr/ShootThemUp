// Shoot Them Uo Game. All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "STUAIController.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAIController : public AAIController
{
    GENERATED_BODY()

protected:
    virtual void OnPossess(APawn* InPawn) override;
};
