// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/GameInstance.h"

/** Module interface for this game's loading screens */
class ICustomMovement : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
	static inline ICustomMovement& Get()
	{
		return FModuleManager::LoadModuleChecked<ICustomMovement>("CustomMovement");
	}



};