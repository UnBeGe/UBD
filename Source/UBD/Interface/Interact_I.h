// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interact_I.generated.h"

UINTERFACE(MinimalAPI)
class UInteract_I : public UInterface
{
	GENERATED_BODY()
};

class UBD_API IInteract_I
{
	GENERATED_BODY()

public:


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|ToInteract")
	void InteractRequest(AActor* InteractableActor);
};