// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include <UBD/UBD.h>
#include "UCharacterGameplayAbility.generated.h"


/**
 * 
 */
UCLASS()
class UBD_API UUCharacterGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UUCharacterGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UBDAbilityID AbilityInputID = UBDAbilityID::None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	UBDAbilityID AbilityID = UBDAbilityID::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool AcitvateAbilityOnGranted = false;

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
};
