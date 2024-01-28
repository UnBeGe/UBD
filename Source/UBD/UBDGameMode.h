// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UBDGameMode.generated.h"

UCLASS(minimalapi)
class AUBDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUBDGameMode();

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	UPROPERTY(BlueprintReadOnly)
	TMap<APlayerController*, FString> PlayersId;
};



