// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.generated.h"

USTRUCT(BlueprintType)
struct FBaseItem : public FTableRowBase
{
    GENERATED_BODY()

    FBaseItem();

    // This is the tooltip for our test variable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText DisplayedName;

    // This is the tooltip for our test variable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Name;

    // This is the tooltip for our test variable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Count;

    // This is the tooltip for our test variable.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSoftClassPtr<AActor> ActorReference;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSoftObjectPtr<UTexture2D> Thumbnail;

    FORCEINLINE bool operator==(const FBaseItem& MyDataReference) const
    {
        return MyDataReference.Name == Name && ActorReference == MyDataReference.ActorReference;
    }

    void Destroy();
};
