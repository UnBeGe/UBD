// All rights reserved Daniil Grigoriev

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
