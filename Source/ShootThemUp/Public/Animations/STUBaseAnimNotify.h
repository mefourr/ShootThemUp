// Shoot Them Uo Game. All Right Reserved 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUBaseAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSignature, USkeletalMeshComponent*);

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUBaseAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
		
public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

    FOnNotifiedSignature OnNotified;
};
