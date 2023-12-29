// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "UBD/LOS/LOSComponent.h"
#include "Kismet/KismetRenderingLibrary.h"


/**
 * 
 */
class FLOSCheckRunnable : public FRunnable
{
	mutable FCriticalSection DataGuard;

public:
	// Custom constructor for setting up our thread with its target
	FLOSCheckRunnable(ULOSComponent* InLOS);
	virtual ~FLOSCheckRunnable();

	TArray<FCanvasUVTri> SafeTraceResults;

	// FRunnable functions
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;
	// FRunnable



protected:
	FRunnableThread* Thread = nullptr;
	ULOSComponent* LOS;

	bool Loop = true;
};


