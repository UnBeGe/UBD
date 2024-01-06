// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "AuthorizationComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FConfirmEMAILRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmailConfirmed);
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UAuthorizationComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()

private:
	const FString RegisterUrl = "http://194.169.160.140/api/PlayersAuthorization/Register/";
	const FString ConfirmUrl = "http://194.169.160.140/api/PlayersAuthorization/Confirm";

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
