// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UBDAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UUBDAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);
/**
 * 
 */
UCLASS()
class UBD_API UUBDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	bool CharacterAbilityesGiven = false;
	bool StartupEffectsApplied = false;

	UPROPERTY(BlueprintAssignable)
	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UUBDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

	UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
	void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
};
