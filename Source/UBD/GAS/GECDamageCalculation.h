// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GECDamageCalculation.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API UGECDamageCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	UGECDamageCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
