// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "ServerRequestComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerCheckResult, bool, Allowed);

UENUM(BlueprintType)
enum class EServerStatus : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	WaitForPlayer = 1 UMETA(DisplayName = "WaitForPlayer"),
	Starting = 2 UMETA(DisplayName = "Starting"),
	Ingame = 3 UMETA(DisplayName = "InGame"),
	Finishing = 4 UMETA(DisplayName = "Finishing")
};
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UServerRequestComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()

private:
	const FString FindServerUrl = "http://194.169.160.140/api/ServerManager/CanPlayOnServer/";
protected:
	void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FPlayerCheckResult OnCheckPlayerResult;


	UFUNCTION(BlueprintCallable)
	void IsPlayerAllowed(int PlayerId, int ServerId);

	UFUNCTION(BlueprintCallable)
	void ChangePlayerStatus(EServerStatus NewStatus, int ServerId);

};
