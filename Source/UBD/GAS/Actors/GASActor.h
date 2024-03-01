// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GASActor.generated.h"

UCLASS()
class UBD_API AGASActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGASActor();


	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetStamina() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxStamina() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UUBDAbilitySystemComponent* AbilitySystemComponent;

	float Level = 1.f;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	FGameplayTag DeadTag;

	UPROPERTY(Transient)
	class UAdventureAttributeSet* AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	void InitializeStartingValues();

	virtual void SetHealth(float Health);

	virtual void SetStamina(float Stamina);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
