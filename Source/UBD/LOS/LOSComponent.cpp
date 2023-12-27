// Fill out your copyright notice in the Description page of Project Settings.


#include "LOSComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
#include "Async/Async.h"
#include "AsyncCheck/AsyncCheck.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

// Sets default values for this component's properties
ULOSComponent::ULOSComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULOSComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		UWorld* World = GetWorld();
		//Debug line traces
		//World->GetTimerManager().SetTimer(CheckTimerHandle, this, &ULOSComponent::CheckLOS, CheckRate, true);
		
		if (FPlatformProcess::SupportsMultithreading())
		{
			LOSViewChackRun = MakeShared<FLOSCheckRunnable>(this);
		}
		Pawn = Cast<APawn>(GetOwner());

		
	}
	
	// ...
}

void ULOSComponent::BeginDestroy()
{
	if (LOSViewChackRun.IsValid())
	{
		LOSViewChackRun->Stop();
	}
	Super::BeginDestroy();
}


// Called every frame
void ULOSComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Pawn)
	{
		UKismetMaterialLibrary::SetVectorParameterValue(this, Collection, ParameterName, FLinearColor(Pawn->GetActorLocation()));
	}
	// ...
}

void ULOSComponent::CheckLOS()
{
	AsyncTask(ENamedThreads::GameThread, [this]() {
		FVector OwnerLocation = GetOwner()->GetActorLocation() - (GetOwner()->GetActorForwardVector() * 15);
		FVector InitialRotation = FRotator(0, NumTraces * DegreesPerTrace / -2.f, 0).RotateVector(GetOwner()->GetActorForwardVector());
		TArray<FVector> TraceResults;
		for (int32 i = 0; i < NumTraces; i++)
		{
			FVector RotatedVector = FRotator(0, i * DegreesPerTrace, 0).RotateVector(InitialRotation);
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());
			
			if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation + OffsetVectorStartTrace, ((RotatedVector * VisionLenght * 2) + OwnerLocation) , CollisionToTrace, Params))
			{
				DrawDebugLine(GetWorld(), OwnerLocation + OffsetVectorStartTrace, Hit.Location, FColor::Red, false, CheckRate);
			}
			else
			{
				DrawDebugLine(GetWorld(), OwnerLocation + OffsetVectorStartTrace, ((RotatedVector * VisionLenght * 2) + OwnerLocation) - OffsetVectorStartTrace, FColor::Red, false, CheckRate);
			}
		}
		
		if (this)
		{
			/*
			AsyncTask(ENamedThreads::GameThread, [this, Triangles]() {

				UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget);
				UCanvas* Canvas;
				FVector2D Size;
				FDrawToRenderTargetContext Context;
				UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);
				if (Canvas && this)
				{
					Canvas->K2_DrawTriangle(nullptr, Triangles);
				}
				else
				{
					UWorld* World = GetWorld();
					if (World)
					{
						World->GetTimerManager().ClearTimer(this->CheckTimerHandle);
					}
				}
				UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
				
				
				});
				*/
		}
		
		});
	
	
	
}

