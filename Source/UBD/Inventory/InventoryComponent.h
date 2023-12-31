// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UBD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged, BlueprintReadOnly, VisibleAnywhere)
	TArray<FBaseItem> Items;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, reliable)
	void AddItem(FBaseItem ItemToAdd);

	UFUNCTION(BlueprintCallable, Server, reliable)
	void RemoveItems(FBaseItem ItemToRemove);

	UFUNCTION(BlueprintCallable)
	void FindItem(FBaseItem ItemToFind, bool& Finded);

	UFUNCTION()
	void OnRep_InventoryChanged();
	
};
