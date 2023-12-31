// All rights reserved Daniil Grigoriev


#include "UBDPlayerState.h"
#include "UBD/GAS/UBDAbilitySystemComponent.h"

AUBDPlayerState::AUBDPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UUBDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAdventureAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.0f;

	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* AUBDPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAdventureAttributeSet* AUBDPlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}

bool AUBDPlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void AUBDPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	// TODO -- Implement hud later
}

float AUBDPlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AUBDPlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

int32 AUBDPlayerState::GetCharacterLevel() const
{
	return AttributeSet->GetLevel();
}

void AUBDPlayerState::BeginPlay()
{
	Super::BeginPlay();
	if (AbilitySystemComponent)
	{
		HeatlhChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AUBDPlayerState::HealthChanged);
		MaxHeatlhChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AUBDPlayerState::MaxHealthChanged);
		CharacterLevelChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetLevelAttribute()).AddUObject(this, &AUBDPlayerState::LevelChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUBDPlayerState::StanTagChanged);
	}
}

void AUBDPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed!"));
}

void AUBDPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed!"));
}

void AUBDPlayerState::LevelChanged(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Level Changed!"));
}

void AUBDPlayerState::StanTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	UE_LOG(LogTemp, Warning, TEXT("Max Health Changed!"));
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}
