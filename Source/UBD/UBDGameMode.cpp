// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBDGameMode.h"
#include "UBDPlayerController.h"
#include "UBDCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AUBDGameMode::AUBDGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AUBDPlayerController::StaticClass();
	/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	*/
}

FString AUBDGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	PlayersId.Add(NewPlayerController, UGameplayStatics::ParseOption(Options, TEXT("Id")));
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}
