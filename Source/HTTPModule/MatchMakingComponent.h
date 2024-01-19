// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "MatchMakingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerFinded, FString, Host);
/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UMatchMakingComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()

private:
	const FString FindServerUrl = "http://194.169.160.140/api/ServerManager/WantsToPlay/";
protected:
	void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FServerFinded OnServerFinded;


	UFUNCTION(BlueprintCallable)
	void FindServer(int Id);

};
