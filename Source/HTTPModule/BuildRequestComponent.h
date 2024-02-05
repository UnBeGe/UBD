// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "BuildRequestComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FBuildFinded, FString, Ability1, FString, Ability2, FString, Ability3);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuildSaved);
/**
 * 
 */
UCLASS()
class HTTPMODULE_API UBuildRequestComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()
private:
	const FString SaveBuildUrl = "http://194.169.160.140/api/Item/SaveBuild/";
	const FString GetBuildUrl = "http://194.169.160.140/api/Item/GetBuild/";
protected:
	void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) override;
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBuildFinded OnBuildFinded;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBuildSaved OnBuildSaved;

	UFUNCTION(BlueprintCallable)
	void GetAbilities(int PlayerId);

	UFUNCTION(BlueprintCallable)
	void SaveAbilities(int PlayerId, FString SessionId);
};
