// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "AuthorizationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfirmEMAILRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmailConfirmed);
/**
 * 
 */
UCLASS()
class CUSTOMMOVEMENT_API UAuthorizationComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()

protected:
	void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) override;

	FString LastEMAIL;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FConfirmEMAILRequested OnConfirmEmail;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FEmailConfirmed OnEmailConfirmed;

	UFUNCTION(BlueprintCallable)
	void RegistrationRequest(FString Password, FString Login, FString EMAIL);

	UFUNCTION(BlueprintCallable)
	void EmailConfirmRequest(FString Code);
};
