// Fill out your copyright notice in the Description page of Project Settings.


#include "LOSComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/Canvas.h"
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
	GetWorld()->GetTimerManager().SetTimer(CheckTimerHandle, this, &ULOSComponent::CheckLOS, CheckRate, true);
	// ...
}


// Called every frame
void ULOSComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULOSComponent::CheckLOS()
{
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector InitialRotation = FRotator(0,NumTraces*DegreesPerTrace/-2.f,0).RotateVector(GetOwner()->GetActorForwardVector());
	TArray<FVector> TraceResults;
	for (int32 i = 0; i < NumTraces; i++)
	{
		FVector RotatedVector = FRotator(0, i*DegreesPerTrace, 0).RotateVector(InitialRotation);
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		//DrawDebugLine(GetWorld(), OwnerLocation, RotatedVector * VisionLenght + OwnerLocation, FColor::Red, false, CheckRate);
		if (GetWorld()->LineTraceSingleByChannel(Hit, OwnerLocation, RotatedVector * VisionLenght + OwnerLocation, CollisionToTrace, Params))
		{
			TraceResults.Add(Hit.Location + RotatedVector * 10);
		}
		else
		{
			TraceResults.Add(RotatedVector * VisionLenght + OwnerLocation);
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
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTarget);
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, RenderTarget, Canvas, Size, Context);
	Canvas->K2_DrawTriangle(nullptr, Triangles);
}

