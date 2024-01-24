// All rights reserved Daniil Grigoriev


#include "UBDFunctionLibrary.h"
#include "SocketSubsystem.h"
#include "GenericPlatform/GenericPlatformMisc.h"

bool UUBDFunctionLibrary::IsEditor()
{
	FModuleManager& ModuleManager = FModuleManager::Get();
	return WITH_EDITOR && ModuleManager.IsModuleLoaded("LevelEditor");
}

void UUBDFunctionLibrary::PlayVAT(UStaticMeshComponent* Mesh)
{
	float DeltaSeconds = 0.0f;
	if (Mesh)
	{
		UWorld* World = Mesh->GetWorld();
		DeltaSeconds = World->GetTimeSeconds();
	}
	else
	{
		return;
	}
	Mesh->SetScalarParameterValueOnMaterials("Playback Speed", 1.f);
	Mesh->SetScalarParameterValueOnMaterials("Game Time at First Frame", DeltaSeconds);
}

void UUBDFunctionLibrary::PlayVATFrame(UStaticMeshComponent* Mesh, float Frame)
{
	Mesh->SetScalarParameterValueOnMaterials("Display Frame", Frame);
}

void UUBDFunctionLibrary::RequestExitServer(const UObject* WorldContextObject, bool Force)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World ? (World->GetNetMode() != NM_Client) : false)
	{
		FGenericPlatformMisc::RequestExit(Force);
	}
	
}

const FString UUBDFunctionLibrary::GetHost(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			bool canBind = false;
			TSharedRef<FInternetAddr> localIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
			return (localIp->IsValid() ? localIp->ToString(false) : World->URL.Host);
			//return World->URL.Host;
		}
	}
	return "WorldContextObject is FALSE";
}


const FString UUBDFunctionLibrary::GetPortNumber(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UWorld* World = WorldContextObject->GetWorld())
		{
			return FString::FromInt(World->URL.Port);
		}
	}
	return "WorldContextObject is FALSE";
}

const int32 UUBDFunctionLibrary::GetServerId()
{
	int32 ServerId = -1;
	if (FParse::Value(FCommandLine::Get(), TEXT("ServerId="), ServerId))
	{
		return ServerId;
	}
	return -1;
}
