// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UBD.h"
#include "AbilitySystemInterface.h"
#include "Public/UBDPlayerState.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "GAS/Character/UCharacterGameplayAbility.h"
#include "CustomMovement/CustomMovementComponent.h"
#include "UBDPlayerController.h"
#include "UBDCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AUBDCharacter*, Character);

UCLASS(Blueprintable)
class AUBDCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AUBDCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(BlueprintAssignable)
	FCharacterDiedDelegate OnCharacterDied;

	UPROPERTY()
	AUBDPlayerController* UBDController;

	UFUNCTION(BlueprintCallable)
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetAbilityLevel(UBDAbilityID AbilityID) const;

	virtual void RemoveCharacterAbilities();

	virtual void Die();
	
	UFUNCTION(BlueprintCallable)
	virtual void FinishDying();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	UCustomMovementComponent* CustomMovementComponent;

	bool ASCInputBound = false;

	TWeakObjectPtr<class UUBDAbilitySystemComponent> AbilitySystemComponent;

	TWeakObjectPtr<class UAdventureAttributeSet> AttributeSet;

	FGameplayTag DeadTag;
	FGameplayTag EffectsRemoveOnDeathTag;

	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText CharatcerName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<class UUCharacterGameplayAbility>> CharacterAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	virtual void AddCharacerAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	void InitializeStartingValues(AUBDPlayerState* PS);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for looking input */
	void Sprint(const FInputActionValue& Value);

	virtual void SendAbilityLocalInput(const FInputActionValue& Value, int32 AbilityID);

	virtual void SetHealth(float Health);

	virtual void SetStamina(float Stamina);

	void BindASCInput();

private:
	FVector CachedDestination;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

};

