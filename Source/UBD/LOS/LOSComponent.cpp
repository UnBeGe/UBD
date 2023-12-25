// Fill out your copyright notice in the Description page of Project Settings.


#include "LOSComponent.h"

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
	FVector InitialRotation = FRotator(0,NumTraces*DegreesPerTrace/-2.f,0).RotateVector(GetOwner()->GetActorLocation());
	TArray<FVector> TraceResults;
	for (int32 i = 0; i < NumTraces; i++)
	{
		FVector RotatedVector = FRotator(0, i*DegreesPerTrace, 0).RotateVector(InitialRotation);
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetOwner());
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), RotatedVector * VisionLenght + GetOwner()->GetActorLocation(), CollisionToTrace, Params))
		{
			TraceResults.Add(Hit.Location + RotatedVector * 10);
		}
		else
		{
			TraceResults.Add(RotatedVector * VisionLenght + GetOwner()->GetActorLocation());
		}
	}
	for (int32 i = 0; i < TraceResults.Num() - 2; i++)
	{

	}
}

