// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerCameraManager.h"
#include "CustomMovementCharacter.h"
#include "Components/CapsuleComponent.h"

ACustomPlayerCameraManager::ACustomPlayerCameraManager()
{

}

void ACustomPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ACustomMovementCharacter* CustomCharacter = Cast<ACustomMovementCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UCustomMovementComponent* CharacterMovement = Cast<UCustomMovementComponent>(CustomCharacter->GetCharacterMovement());
		FVector TargetCrouchOffset = FVector(0, 0, CharacterMovement->GetCrouchedHalfHeight() - CustomCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()); // Default will be negative value
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));
		if (CharacterMovement->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (CharacterMovement->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}
	}
}
