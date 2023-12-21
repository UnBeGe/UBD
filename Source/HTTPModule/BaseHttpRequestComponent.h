// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "Components/ActorComponent.h"
#include "BaseHttpRequestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLostConnection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConnectionSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResponseReceived, bool, Success, FString, Details);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class  UBaseHttpRequestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseHttpRequestComponent();

	//Delegate for call ivent if connection lost
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FLostConnection OnLostConnection;

	//Delegate for call ivent if connection lost
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FConnectionSuccess OnConnectionSuccess;
	//Response delegate
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FResponseReceived OnResponse;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Call on check connection request complete, if connection lost log and call lost connetion delegate call
	virtual void LostConnectionCheck(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	virtual void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	virtual void SendRequest(FString Url, TSharedRef<FJsonObject> RequestObj, FString Verb = "POST", FString HeaderName = "Content-Type", FString HeaderValue = "application/json");


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable)
		virtual void CheckConnection(FString Url);

	
};
