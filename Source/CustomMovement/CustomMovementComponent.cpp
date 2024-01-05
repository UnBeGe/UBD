// All rights reserved Daniil Grigoriev


#include "CustomMovementComponent.h"
#include "CustomMovementCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

void UCustomMovementComponent::OnRep_DashStart()
{
	if (Proxy_bDashStart)
	{
		DashStartDelegate.Broadcast();
	}
}

void UCustomMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse; //remove z velocity
	SetMovementMode(MOVE_Custom, CMOVE_Slide); // changing movement mode
}

void UCustomMovementComponent::ExitSlide()
{
	//bWantsToCrouch = false;

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);
	SetMovementMode(MOVE_Walking);
}

void UCustomMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(SlideMinSpeed, 2))
	{
		ExitSlide();
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	//Gravity
	Velocity += SlideGravityForce * FVector::DownVector * deltaTime;

	//Strafe

	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > .5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	//Calculate velocity
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime, SlideFriction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	//perform move

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime;
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.Normal).GetSafeNormal();
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();
	//Actual update capsule
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);
		//Actual update capsule if previus hit something
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(SlideMinSpeed, 2))
	{
		ExitSlide();
	}
	//update outgoing velocity
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}
}

bool UCustomMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	FName ProfileName = TEXT("BlockAll");
	return GetWorld()->LineTraceSingleByProfile(Hit, Start, End, ProfileName, CustomMovementCharacterOwner->GetIgnoredCharacterParams());
}

bool UCustomMovementComponent::CanDash() const
{
	return IsWalking() && !IsCrouching();
}

void UCustomMovementComponent::PerformDash()
{
	DashStartTime = GetWorld()->GetTimeSeconds();

	FVector DashDirection = (Acceleration.IsNearlyZero() ? UpdatedComponent->GetForwardVector() : Acceleration).GetSafeNormal2D();
	Velocity = DashImpulse * (DashDirection + FVector::UpVector * 0.1f);

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(DashDirection, FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, false, Hit);

	SetMovementMode(MOVE_Falling);

	DashStartDelegate.Broadcast();
}

void UCustomMovementComponent::OnDashCoolDownFinished()
{
	Safe_bWantsToDash = true;
}

UCustomMovementComponent::UCustomMovementComponent()
{
}

void UCustomMovementComponent::SprintPressed()
{
	Safe_WantsToSprint = true;
}

void UCustomMovementComponent::SprintReleased()
{
	Safe_WantsToSprint = false;
}

float UCustomMovementComponent::GetTotalSpeedModificator()
{
	TArray<float> Modificators;
	MovementModificators.GenerateValueArray(Modificators);
	float ResultModificator = 1;
	for (float Modificator : Modificators)
	{
		ResultModificator *= Modificator;
	}
	return ResultModificator;
}

void UCustomMovementComponent::RemoveModificator(FString ModificatorName)
{
	MovementModificators.Remove(ModificatorName);
	TotalModificator = GetTotalSpeedModificator();
}

bool UCustomMovementComponent::IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == InCustomMovementMode;;
}

void UCustomMovementComponent::CrouchPressed()
{
	bWantsToCrouch = true;
}

void UCustomMovementComponent::CrouchReleased()
{
	bWantsToCrouch = false;
}

void UCustomMovementComponent::DashPressed()
{
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - DashStartTime >= DashCooldownDuration)
	{
		Safe_bWantsToDash = true;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DashCooldown, this, &UCustomMovementComponent::OnDashCoolDownFinished, DashCooldownDuration - (CurrentTime - DashStartTime));
	}
}

void UCustomMovementComponent::DashhReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_DashCooldown);
	Safe_bWantsToDash = false;
}

void UCustomMovementComponent::AddModificator(FString ModificatorName, float ModificatorValue)
{
	MovementModificators.Add(ModificatorName, ModificatorValue);
	TotalModificator = GetTotalSpeedModificator();
}

void UCustomMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	CustomMovementCharacterOwner = Cast<ACustomMovementCharacter>(GetOwner());
}

void UCustomMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_WantsToSprint = (Flags & FSavedMove_Custom::FLAG_Sprint) != 0;
	Safe_bWantsToDash = (Flags & FSavedMove_Custom::FLAG_Dash) != 0;
}

void UCustomMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	Safe_bPrevWantsToCrouch = bWantsToCrouch;
}

void UCustomMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UCustomMovementComponent, Proxy_bDashStart, COND_SkipOwner);
}

bool UCustomMovementComponent::IsMovingOnGround() const
{
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UCustomMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UCustomMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	//slide
	if (MovementMode == MOVE_Walking && bWantsToCrouch && Safe_bPrevWantsToCrouch)
	{
		FHitResult PotentialSlideSurface;
		if (Velocity.SizeSquared() > pow(SlideMinSpeed, 2) && GetSlideSurface(PotentialSlideSurface))
		{
			EnterSlide();
		}
	}

	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		ExitSlide();
	}

	//dash
	bool bAuthProxy = CharacterOwner->HasAuthority() && !CharacterOwner->IsLocallyControlled();
	if (Safe_bWantsToDash && CanDash())
	{
		if (!bAuthProxy || GetWorld()->GetTimeSeconds() - DashStartTime > AuthDashCooldownDuration)
		{
			PerformDash();
			Safe_bWantsToDash = false;
			Proxy_bDashStart = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client tried to cheat"));
		}
	}
	else
	{
		Proxy_bDashStart = false;
	}
	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UCustomMovementComponent::UpdateCharacterStateAfterMovement(float DeltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(DeltaSeconds);

}

void UCustomMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		PhysSlide(deltaTime, Iterations);
		break;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid movement mode"))
	}
}

float UCustomMovementComponent::GetMaxSpeed() const
{
	
	if (IsMovementMode(MOVE_Walking) && Safe_WantsToSprint && !IsCrouching())
	{
		return Sprint_MaxWalkSpeed * TotalModificator;
	}
	//if (MovementMode != MOVE_Custom)
	{
		return Super::GetMaxSpeed() * TotalModificator;
	}
}

bool UCustomMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}

FNetworkPredictionData_Client* UCustomMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

		if (ClientPredictionData == nullptr)
		{
			UCustomMovementComponent* MutableThis = const_cast<UCustomMovementComponent*>(this);

			MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_Custom(*this);
			MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
			MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
		}
	return ClientPredictionData;
}

bool UCustomMovementComponent::FSavedMove_Custom::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_Custom* NewCustomMove = static_cast<FSavedMove_Custom*>(NewMove.Get());

	if (Saved_WantsToSprint != NewCustomMove->Saved_WantsToSprint)
	{
		return false;
	}
	if (Saved_bWantsToDash != NewCustomMove->Saved_bWantsToDash)
	{
		return false;
	}

	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void UCustomMovementComponent::FSavedMove_Custom::Clear()
{
	FSavedMove_Character::Clear();

	Saved_WantsToSprint = 0;
	Saved_bPrevWantsToCrouch = 0;
	Saved_bWantsToDash = 0;
	Saved_bWantsToProne = 0;
}

uint8 UCustomMovementComponent::FSavedMove_Custom::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_WantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	if (Saved_bWantsToDash )
	{
		Result |= FLAG_Dash;
	}

	return Result;
}
//client
void UCustomMovementComponent::FSavedMove_Custom::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UCustomMovementComponent* CharacterMovement = Cast<UCustomMovementComponent>(C->GetCharacterMovement());

	Saved_WantsToSprint = CharacterMovement->Safe_WantsToSprint;

	Saved_bPrevWantsToCrouch = CharacterMovement->Safe_bPrevWantsToCrouch;

	Saved_bWantsToDash = CharacterMovement->Safe_bWantsToDash;
}
//server
void UCustomMovementComponent::FSavedMove_Custom::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	UCustomMovementComponent* CharacterMovement = Cast<UCustomMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_WantsToSprint = Saved_WantsToSprint;

	CharacterMovement->Safe_bPrevWantsToCrouch = Saved_bPrevWantsToCrouch;

	CharacterMovement->Safe_bWantsToDash = Saved_bWantsToDash;
}

UCustomMovementComponent::FNetworkPredictionData_Client_Custom::FNetworkPredictionData_Client_Custom(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UCustomMovementComponent::FNetworkPredictionData_Client_Custom::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_Custom());
}
