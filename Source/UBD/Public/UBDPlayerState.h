// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "UBD/GAS/AdventureAttributeSet.h"
#include "UBDPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API AUBDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AUBDPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UAdventureAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	int32 GetCharacterLevel() const;
protected:

	UPROPERTY()
	class UAdventureAttributeSet* AttributeSet;

	FGameplayTag DeadTag;

	FDelegateHandle HeatlhChangedDelegateHandle;
	FDelegateHandle MaxHeatlhChangedDelegateHandle;
	FDelegateHandle CharacterLevelChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void LevelChanged(const FOnAttributeChangeData& Data);
	virtual void StanTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UUBDAbilitySystemComponent* AbilitySystemComponent;
};
