// Copyright Epic Games, Inc. All Rights Reserved.

#include "UBDCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GAS/Character/UCharacterGameplayAbility.h"
#include "GAS/UBDAbilitySystemComponent.h"
#include "GAS/AdventureAttributeSet.h"
#include "CustomMovement/CustomMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/UBDPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"


AUBDCharacter::AUBDCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustomMovementComponent>(CharacterMovementComponentName))
{
	CustomMovementComponent = Cast<UCustomMovementComponent>(GetCharacterMovement());
	CustomMovementComponent->SetIsReplicated(true);

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectsRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("State.RemoveOnDeath"));

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->SetIsReplicated(true);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

bool AUBDCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

int32 AUBDCharacter::GetAbilityLevel(UBDAbilityID AbilityID) const
{
	return 1;
}

void AUBDCharacter::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || !AbilitySystemComponent->CharacterAbilityesGiven)
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
	if (Spec.SourceObject == this && CharacterAbilities.Contains(Spec.Ability->GetClass()))
		{
		AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}

	AbilitySystemComponent->CharacterAbilityesGiven = false;
}

void AUBDCharacter::Die()
{
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->CancelAbilities();

		FGameplayTagContainer EffectsTagsToRemove;
		EffectsTagsToRemove.AddTag(EffectsRemoveOnDeathTag);
		int32 NumEffectsEremoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectsTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);

	}
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
	
}

void AUBDCharacter::FinishDying()
{
	Destroy();
}

float AUBDCharacter::GetHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float AUBDCharacter::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetLevel();
	}
	return 0.0f;
}

float AUBDCharacter::GetMaxHealth() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

float AUBDCharacter::GetStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}
	return 0.0f;
}

float AUBDCharacter::GetMaxStamina() const
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.0f;
}

void AUBDCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UAbilitySystemComponent* AUBDCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void AUBDCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AUBDPlayerState* PS = GetPlayerState<AUBDPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);
		BindASCInput();
		//InitializeAttributes();
	}
}

void AUBDCharacter::AddCharacerAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->CharacterAbilityesGiven)
	{
		return;
	}

	for (TSubclassOf<UUCharacterGameplayAbility>& StartupAbility: CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->CharacterAbilityesGiven = true;
}

void AUBDCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the characers blueprint."), *FString(__FUNCTION__), *GetName());
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);

	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

void AUBDCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->StartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);

		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->StartupEffectsApplied = true;
}

void AUBDCharacter::InitializeStartingValues(AUBDPlayerState* PS)
{

	AbilitySystemComponent = Cast<UUBDAbilitySystemComponent>(PS->GetAbilitySystemComponent());

	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

	AttributeSet = PS->GetAttributeSetBase();

	AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

	InitializeAttributes();

	SetHealth(GetMaxHealth());

	SetStamina(GetMaxStamina());

}

void AUBDCharacter::Move(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (UBDController == nullptr)
	{
		UBDController = Cast<AUBDPlayerController>(Controller);
	}
	if (UBDController != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = CameraBoom->GetComponentRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		
		

		//get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUBDCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{



		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AUBDCharacter::Sprint);
		EnhancedInputComponent->BindAction(AbilityOneAction, ETriggerEvent::Triggered, this, &AUBDCharacter::AbilityOne);
		EnhancedInputComponent->BindAction(AbilityTwoAction, ETriggerEvent::Triggered, this, &AUBDCharacter::AbilityTwo);
		EnhancedInputComponent->BindAction(AbilityThreeAction, ETriggerEvent::Triggered, this, &AUBDCharacter::AbilityThree);
		EnhancedInputComponent->BindAction(ConfirmAction, ETriggerEvent::Triggered, this, &AUBDCharacter::Confirm);
		EnhancedInputComponent->BindAction(CancelAction, ETriggerEvent::Triggered, this, &AUBDCharacter::Cancel);


		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUBDCharacter::Move);

		//Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomMovementCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	BindASCInput();
}

void AUBDCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AUBDPlayerState* PS = GetPlayerState<AUBDPlayerState>();
	if (PS)
	{
		InitializeStartingValues(PS);

		
		AddStartupEffects();
		AddCharacerAbilities();
	}
}

void AUBDCharacter::Look(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void AUBDCharacter::Sprint(const FInputActionValue & Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Sprint));
}

void AUBDCharacter::AbilityOne(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Ability1));
}

void AUBDCharacter::AbilityThree(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Ability3));
}

void AUBDCharacter::AbilityTwo(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Ability2));
}

void AUBDCharacter::Confirm(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Confirm));
}

void AUBDCharacter::Cancel(const FInputActionValue& Value)
{
	if (!IsAlive())
	{
		return;
	}

	SendAbilityLocalInput(Value, static_cast<int32>(UBDAbilityID::Cancel));
}

void AUBDCharacter::SendAbilityLocalInput(const FInputActionValue& Value, int32 AbilityID)
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}
	bool Pressed = Value.Get<bool>();
	if (Pressed)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(AbilityID);
	}
	else
	{
		AbilitySystemComponent->AbilityLocalInputReleased(AbilityID);
	}
}

void AUBDCharacter::SetHealth(float Health)
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(Health);
	}
}

void AUBDCharacter::SetStamina(float Stamina)
	{
		if (AttributeSet.IsValid())
		{
			AttributeSet->SetStamina(Stamina);
		}
	}

void AUBDCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/UBD"), FName("UBDAbilityID"));
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(UBDAbilityID::Confirm), static_cast<int32>(UBDAbilityID::Cancel)));

		ASCInputBound = true;
	}
}
