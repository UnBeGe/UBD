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
					TraceResults.Add(Hit.Location + -1 * Hit.Normal * LOS->HitOffset);
				}
				else
				{
					TraceResults.Add(RotatedVector * LOS->VisionLenght * 2 + OwnerLocation);
				}
			}
			TArray<FCanvasUVTri> Triangles;
			float Buffer = (TraceResults.Num() * LOS->BufferPercent) / 100;
			float EndBuffer = TraceResults.Num() - Buffer;

			for (int32 i = 0; i < TraceResults.Num() - 2; i++)
			{
				
				float V0Color = 1;
				if (i <= Buffer)
				{

					V0Color = (i/Buffer);
				}
				else if (i >= EndBuffer)
				{
					
					V0Color = (1.0f - ((i - EndBuffer) / Buffer));
				}
				FCanvasUVTri Tri;
				float V1Color = (V0Color *1) - ((TraceResults[i] - OwnerLocation).Length() / (LOS->VisionLenght * 2));
				//V1Color *= V0Color;
				float V2Color = (V0Color * 1) - ((TraceResults[i + 1] - OwnerLocation).Length() / (LOS->VisionLenght * 2));
				//V2Color *= V0Color;
				Tri.V0_Color = FLinearColor(V0Color, V0Color, V0Color, 0);
				Tri.V1_Color = FLinearColor(V1Color, V1Color, V1Color, 0);
				Tri.V2_Color = FLinearColor(V2Color, V2Color, V2Color, 0);
				Tri.V0_Pos = LOS->OffsetVector;
				Tri.V1_Pos = LOS->OffsetVector + FVector2D(TraceResults[i] - OwnerLocation);
				Tri.V2_Pos = LOS->OffsetVector + FVector2D(TraceResults[i + 1] - OwnerLocation);
				Triangles.Add(Tri);
			}
			if (LOS)
			{
				AsyncTask(ENamedThreads::GameThread, [this, Triangles]() {
					if (this && IsValid(LOS) && Loop)
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
