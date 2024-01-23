// All rights reserved Daniil Grigoriev

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UBDFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UBD_API UUBDFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public: 
	UFUNCTION(BlueprintCallable, Category = "Editor")
	static bool IsEditor();

	UFUNCTION(BlueprintCallable, Category = "HoudiniVFX")
	static void PlayVAT(UStaticMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable, Category = "HoudiniVFX")
	static void PlayVATFrame(UStaticMeshComponent* Mesh, float Frame);

	UFUNCTION(BlueprintCallable, Category = "Server", meta = (WorldContext = "WorldContextObject"))
	static void RequestExitServer(const UObject* WorldContextObject, bool Force = false);
};
