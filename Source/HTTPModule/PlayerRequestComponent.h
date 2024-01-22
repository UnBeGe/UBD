// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseHttpRequestComponent.h"
#include "PlayerRequestComponent.generated.h"


UENUM(BlueprintType)
enum class EPlayerStatus : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	offline = 1 UMETA(DisplayName = "Offline"),
	online = 2 UMETA(DisplayName = "Online"),
	ingame = 3 UMETA(DisplayName = "InGame"),
	searching = 4 UMETA(DisplayName = "Searching")
};

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class  UPlayerRequestComponent : public UBaseHttpRequestComponent
{
	GENERATED_BODY()

private:
	const FString FindServerUrl = "http://194.169.160.140/api/ServerManager/WantsToPlay/";
	const FString ChangePlayerStatusUrl = "http://194.169.160.140/api/Status/CangePlayerStatus/";
protected:
	void OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully) override;

public:


	UFUNCTION(BlueprintCallable)
	void ChangePlayerStatus(EPlayerStatus NewStatus, int PlayerId);

};
