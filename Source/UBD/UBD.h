// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUBD, Log, All);

UENUM(BlueprintType)
enum class UBDAbilityID : uint8
{
	None		UMETA(DisplayName = "None"),
	Confirm		UMETA(DisplayName = "Confirm"),
	Cancel		UMETA(DisplayName = "Cancel"),
	Sprint		UMETA(DisplayName = "Sprint"),
	Ability1	UMETA(DisplayName = "Ability1"),
	Ability2	UMETA(DisplayName = "Ability2"),
	Ability3	UMETA(DisplayName = "Ability3"),
};
