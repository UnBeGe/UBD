// All rights reserved Daniil Grigoriev


#include "UBDBaseEnemy.h"
#include "SkeletalMeshComponentBudgeted.h"
#include "UBD/GAS/UBDAbilitySystemComponent.h"

AUBDBaseEnemy::AUBDBaseEnemy(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<USkeletalMeshComponentBudgeted>(ACharacter::MeshComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UUBDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

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
