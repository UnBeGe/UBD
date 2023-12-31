// All rights reserved Daniil Grigoriev


#include "UBDAssetManger.h"
#include "AbilitySystemGlobals.h"

void UUBDAssetManger::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
