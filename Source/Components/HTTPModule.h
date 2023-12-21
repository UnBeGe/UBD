// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Engine/GameInstance.h"

/** Module interface for this game's loading screens */
class IHTTPModule : public IModuleInterface
{
public:
	/** Loads the module so it can be turned on */
	static inline IHTTPModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IHTTPModule>("HTTPModule");
	}



};