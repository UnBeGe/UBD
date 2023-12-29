// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LOSComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBD_API ULOSComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULOSComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginDestroy() override;

	FTimerHandle CheckTimerHandle;


	TSharedPtr<class FLOSCheckRunnable> LOSViewChackRun;

	void CheckLOS();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ParameterName;

	APawn* Pawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMaterialParameterCollection* Collection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CheckRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DegreesPerTrace = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HitOffset = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 NumTraces = 32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float VisionLenght = 500.0f;
	//Should be less then 50
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BufferPercent = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector2D OffsetVector = FVector2D(900, 900);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector OffsetVectorStartTrace = FVector(0,0, 80);

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	TEnumAsByte<ECollisionChannel> CollisionToTrace = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTextureRenderTarget2D* RenderTarget;

	void UpdateRenderTarget(TArray<FCanvasUVTri> Triangles);
};

