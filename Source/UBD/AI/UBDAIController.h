// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UBDAIController.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API AUBDAIController : public AAIController
{
	GENERATED_BODY()
protected:

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGenericTeamId TeamId = 0;

	UPROPERTY(BlueprintReadWrite)
	class AUBDBaseEnemy* Agent;
};
