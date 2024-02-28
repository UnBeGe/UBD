// All rights reserved Daniil Grigoriev


#include "GASFunctions.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
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
		UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(ASC->GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
	}
}
