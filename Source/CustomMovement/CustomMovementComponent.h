// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomMovementComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDashStartDelegate);

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None UMETA(Hidden),
	CMOVE_Slide UMETA(DisplayName = "Slide"),
	CMOVE_MAX UMETA(Hidden),
};

/**
 * 
 */
UCLASS()
class CUSTOMMOVEMENT_API UCustomMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FSavedMove_Custom : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_WantsToSprint : 1;

		uint8 Saved_bPrevWantsToCrouch : 1;
	public:
		enum CompressedFlags
		{
			FLAG_Sprint = 0x10,
			FLAG_Dash = 0x20,
			FLAG_Custom_2 = 0x40,
			FLAG_Custom_3 = 0x80,
		};

		uint8 Saved_bWantsToDash : 1;

		uint8 Saved_bWantsToProne : 1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_Custom : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
public:
	UPROPERTY(BlueprintAssignable)
	FDashStartDelegate DashStartDelegate;

protected:
	virtual void InitializeComponent() override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;



	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual float GetMaxSpeed() const;

	bool IsMovementMode(EMovementMode InMovementMode) const;

	TMap<FString, float> MovementModificators;

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UPROPERTY(EditDefaultsOnly)
		float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly)
		float Walk_MaxWalkSpeed;
		//slide
	UPROPERTY(EditDefaultsOnly) 
	float SlideMinSpeed = 350.f;
	UPROPERTY(EditDefaultsOnly) 
	float SlideEnterImpulse = 500.f;
	UPROPERTY(EditDefaultsOnly) 
	float SlideGravityForce = 5000.f;
	UPROPERTY(EditDefaultsOnly) 
	float SlideFriction = 1.3f;

	//Dash
	UPROPERTY(EditDefaultsOnly)
	float DashImpulse = 1000.f;
	UPROPERTY(EditDefaultsOnly)
	float DashCooldownDuration = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float AuthDashCooldownDuration = 0.9f;

//Transient
	UPROPERTY(Transient)
		class ACustomMovementCharacter* CustomMovementCharacterOwner;

	float TotalModificator = 1;

	bool Safe_WantsToSprint;
	bool Safe_bWantsToDash;

	bool Safe_bPrevWantsToCrouch;

	float DashStartTime;

	FTimerHandle TimerHandle_DashCooldown;
//replication
	UPROPERTY(ReplicatedUsing=OnRep_DashStart)
	bool Proxy_bDashStart;

private:
	//on reps
	UFUNCTION() void OnRep_DashStart();


//slide
	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

//dash
	bool CanDash() const;
	void PerformDash();
	void OnDashCoolDownFinished();
public:
	UCustomMovementComponent();

	UFUNCTION(BlueprintCallable)
		void SprintPressed();
	UFUNCTION(BlueprintCallable)
		void SprintReleased();
	UFUNCTION(BlueprintCallable)
		float GetTotalSpeedModificator();
	UFUNCTION(BlueprintCallable)
		void AddModificator(FString ModificatorName,float ModificatorValue);
	UFUNCTION(BlueprintCallable)
		void RemoveModificator(FString ModificatorName);
	UFUNCTION(BlueprintCallable)
		bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	UFUNCTION(BlueprintCallable) 
	void CrouchPressed();
	UFUNCTION(BlueprintCallable) 
	void CrouchReleased();

	UFUNCTION(BlueprintCallable)
	void DashPressed();
	UFUNCTION(BlueprintCallable)
	void DashhReleased();

	
};
