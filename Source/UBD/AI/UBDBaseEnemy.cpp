// All rights reserved Daniil Grigoriev


#include "UBDBaseEnemy.h"
#include "UBD/GAS/UBDAbilitySystemComponent.h"

AUBDBaseEnemy::AUBDBaseEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UUBDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AIAttribSet = CreateDefaultSubobject<UAdventureAttributeSet>(TEXT("AttributeSet"));
}

void AUBDBaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);

		AttributeSet = AIAttribSet;

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);



		InitializeAttributes();

		SetHealth(GetMaxHealth());

		SetStamina(GetMaxStamina());

		AddStartupEffects();

		AddCharacerAbilities();
	}
}
