// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "UBD/UBDCharacter.h"
#include "UBDBaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API AUBDBaseEnemy : public AUBDCharacter
{
	GENERATED_BODY()
	
public:
	AUBDBaseEnemy(const FObjectInitializer& ObjectInitializer);

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(Transient)
	class UAdventureAttributeSet* AIAttribSet;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
	int32 ID = 0;
};
