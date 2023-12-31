// All rights reserved Daniil Grigoriev


#include "BaseItem.h"

FBaseItem::FBaseItem()
{
	DisplayedName = FText::FromString("BaseDisplayName");
	Name = "Base item name";
	Count = -1;
	Thumbnail.Reset();
}

void FBaseItem::Destroy()
{
	Thumbnail.Reset();
}

