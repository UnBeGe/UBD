// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"

FBaseItem::FBaseItem()
{
	DisplayedName = FText::FromString("BaseDisplayName");
	Name = "Base item name";
	Count = -1;
	ActorReference.Reset();
	Thumbnail.Reset();
}

void FBaseItem::Destroy()
{
	ActorReference.Reset();
	Thumbnail.Reset();
}

