// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "GASFunctions.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API UGASFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static void CancelAbilityByTag(UAbilitySystemComponent* ASC, FGameplayTagContainer WithTags, FGameplayTagContainer WithoutTags, UGameplayAbility* Ignore);

	UFUNCTION(BlueprintCallable)
	static void ActivateCUE(UAbilitySystemComponent* ASC, const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
	
};
