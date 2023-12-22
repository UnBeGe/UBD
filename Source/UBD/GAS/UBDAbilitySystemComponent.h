// Fill out your copyright notice in the Description page of Project Settings.

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
};
