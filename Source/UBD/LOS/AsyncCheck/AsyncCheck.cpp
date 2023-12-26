// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncCheck.h"




FLOSCheckRunnable::FLOSCheckRunnable(ULOSComponent* InLOS)
{
	LOS = InLOS;
	Thread = FRunnableThread::Create(this, TEXT("LOSCheckthread"), 0U, EThreadPriority::TPri_Lowest);
}

FLOSCheckRunnable::~FLOSCheckRunnable()
{
	if (Thread != nullptr)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

uint32 FLOSCheckRunnable::Run()
{
	while (Loop)
	{
		if (LOS)
		{
			FVector OwnerLocation = LOS->GetOwner()->GetActorLocation();
			FVector InitialRotation = FRotator(0, LOS->NumTraces * LOS->DegreesPerTrace / -2.f, 0).RotateVector(LOS->GetOwner()->GetActorForwardVector());
			TArray<FVector> TraceResults;
			for (int32 i = 0; i < LOS->NumTraces; i++)
			{
				FVector RotatedVector = FRotator(0, i * LOS->DegreesPerTrace, 0).RotateVector(InitialRotation);
				FHitResult Hit;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(LOS->GetOwner());
				//DrawDebugLine(GetWorld(), OwnerLocation, RotatedVector * VisionLenght + OwnerLocation, FColor::Red, false, CheckRate);
				if (LOS->GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation + LOS->OffsetVectorStartTrace, RotatedVector * LOS->VisionLenght * 2 + OwnerLocation + LOS->OffsetVectorStartTrace, LOS->CollisionToTrace, Params))
				{
					TraceResults.Add(Hit.Location + RotatedVector * LOS->HitOffset);
				}
				else
				{
					TraceResults.Add(RotatedVector * LOS->VisionLenght * 2 + OwnerLocation);
				}
			}
			TArray<FCanvasUVTri> Triangles;
			for (int32 i = 0; i < TraceResults.Num() - 2; i++)
			{
				FCanvasUVTri Tri;
				Tri.V0_Color = FLinearColor(1, 1, 1, 0);
				Tri.V1_Color = FLinearColor(1, 1, 1, 0);
				Tri.V2_Color = FLinearColor(1, 1, 1, 0);
				Tri.V0_Pos = LOS->OffsetVector;
				Tri.V1_Pos = LOS->OffsetVector + FVector2D(TraceResults[i] - OwnerLocation);
				Tri.V2_Pos = LOS->OffsetVector + FVector2D(TraceResults[i + 1] - OwnerLocation);
				Triangles.Add(Tri);
			}
			if (LOS)
			{
				AsyncTask(ENamedThreads::GameThread, [this, Triangles]() {
					if (IsValid(LOS) && Loop)
					{
						UKismetRenderingLibrary::ClearRenderTarget2D(LOS, LOS->RenderTarget);
						UCanvas* Canvas;
						FVector2D Size;
						FDrawToRenderTargetContext Context;
						UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(LOS, LOS->RenderTarget, Canvas, Size, Context);
						if (Canvas && this)
						{
							Canvas->K2_DrawTriangle(nullptr, Triangles);
						}
						else
						{
							UWorld* World = LOS->GetWorld();
							if (World)
							{
								World->GetTimerManager().ClearTimer(LOS->CheckTimerHandle);
							}
						}
						UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(LOS, Context);
					}
					


					});
			}
			FPlatformProcess::Sleep(LOS->CheckRate);
		}
		else
		{
			Loop = false;
		}
		
	}
	
	return 0;
}

void FLOSCheckRunnable::Exit()
{
	LOS = nullptr;
}

void FLOSCheckRunnable::Stop()
{
	Loop = false;
}
