// Shoot Them Uo Game. All Right Reserved

#include "Animations/STUBaseAnimNotify.h"

void USTUBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    
    Super::Notify(MeshComp, Animation);
}