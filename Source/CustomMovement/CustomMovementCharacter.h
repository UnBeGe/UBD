// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CustomMovementComponent.h"
#include "CustomMovementCharacter.generated.h"



UCLASS(config=Game)
class CUSTOMMOVEMENT_API ACustomMovementCharacter : public ACharacter
{
	GENERATED_BODY()

protected:



public:
	ACustomMovementCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	UCustomMovementComponent* CustomMovementComponent;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

public:

	// To add mapping context
	virtual void BeginPlay();


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UInputMappingContext* DefaultMappingContext;


	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> CharacterChildren;
	
	FCollisionQueryParams GetIgnoredCharacterParams() const;
};

