// All rights reserved Daniil Grigoriev


#include "GASFunctions.h"

void UGASFunctions::CancelAbilityByTag(UAbilitySystemComponent* ASC, FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags, UGameplayAbility* Ignore)
{
	if (ASC)
	{
		ASC->CancelAbilities(&WithTags, &WithoutTags, Ignore);
	}
	
}

void UGASFunctions::ActivateCUE(UAbilitySystemComponent* ASC, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters)
{
	if (ASC)
	{
		ASC->ExecuteGameplayCue(GameplayCueTag, GameplayCueParameters);
	}
}
