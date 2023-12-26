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
	AsyncTask(ENamedThreads::AnyBackgroundThreadNormalTask, [this]() {
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector InitialRotation = FRotator(0, NumTraces * DegreesPerTrace / -2.f, 0).RotateVector(GetOwner()->GetActorForwardVector());
		TArray<FVector> TraceResults;
		for (int32 i = 0; i < NumTraces; i++)
		{
			FVector RotatedVector = FRotator(0, i * DegreesPerTrace, 0).RotateVector(InitialRotation);
			FHitResult Hit;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());
			//DrawDebugLine(GetWorld(), OwnerLocation, RotatedVector * VisionLenght + OwnerLocation, FColor::Red, false, CheckRate);
			if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation + OffsetVectorStartTrace, RotatedVector * VisionLenght * 2 + OwnerLocation + OffsetVectorStartTrace, CollisionToTrace, Params))
			{
				TraceResults.Add(Hit.Location + RotatedVector * HitOffset);
			}
			else
			{
				TraceResults.Add(RotatedVector * VisionLenght * 2 + OwnerLocation);
			}
		}
		TArray<FCanvasUVTri> Triangles;
		for (int32 i = 0; i < TraceResults.Num() - 2; i++)
		{
			FCanvasUVTri Tri;
			Tri.V0_Color = FLinearColor(1, 1, 1, 0);
			Tri.V1_Color = FLinearColor(1, 1, 1, 0);
			Tri.V2_Color = FLinearColor(1, 1, 1, 0);
			Tri.V0_Pos = OffsetVector;
			Tri.V1_Pos = OffsetVector + FVector2D(TraceResults[i] - OwnerLocation);
			Tri.V2_Pos = OffsetVector + FVector2D(TraceResults[i + 1] - OwnerLocation);
			Triangles.Add(Tri);
		}
		if (this)
		{
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
		}
		
		});
	
	
	
}

