// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/BaseItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Items);
}

void UInventoryComponent::AddItem_Implementation(FBaseItem ItemToAdd)
{
	if (GetOwner()->HasAuthority())
	{ 
	int32 Index = Items.Find(ItemToAdd);
	if (Index == INDEX_NONE)
	{
		Index = Items.Add(ItemToAdd);
	}
	else
	{
		Items[Index].Count += ItemToAdd.Count;
	}

	return;
	}
	return;
}

void UInventoryComponent::RemoveItems_Implementation(FBaseItem ItemToRemove)
{
	if (GetOwner()->HasAuthority())
	{
		int32 Index = Items.Find(ItemToRemove);
		if (Index == INDEX_NONE)
		{
		}
		else
		{
			Items[Index].Count -= ItemToRemove.Count;
			if (Items[Index].Count <= 0)
			{
				Items.RemoveAt(Index);
			}
		}

		return;
	}
	return;
}

void UInventoryComponent::FindItem(FBaseItem ItemToFind, bool& Finded)
{
		int32 Index = Items.Find(ItemToFind);
		if (Index == INDEX_NONE)
		{
			Finded = false;
		}
		else
		{
			Finded = true;
		}
}

void UInventoryComponent::OnRep_InventoryChanged()
{
}

