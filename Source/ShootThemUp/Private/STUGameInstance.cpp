// Shoot Them Uo Game. All Right Reserved

#include "STUGameInstance.h"
#include "Sound/STUSoundFuncLib.h"

void USTUGameInstance::ToggleVolume()
{
    USTUSoundFuncLib::ToggletSoundClassVolume(MasterSoundClass);
}
