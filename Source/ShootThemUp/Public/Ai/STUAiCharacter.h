// Shoot Them Uo Game. All Right Reserved 

#pragma once

#include "CoreMinimal.h"
#include "Player/STUBaseCharacter.h"
#include "STUAiCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API ASTUAiCharacter : public ASTUBaseCharacter
{
	GENERATED_BODY()
public:
    ASTUAiCharacter(const FObjectInitializer& ObjInit);
};
