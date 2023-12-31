// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UBDAssetManger.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API UUBDAssetManger : public UAssetManager
{
	GENERATED_BODY()
public:
	virtual void StartInitialLoading() override;
};
