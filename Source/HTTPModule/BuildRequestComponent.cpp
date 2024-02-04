// All rights reserved Daniil Grigoriev


#include "BuildRequestComponent.h"

void UBuildRequestComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	//Super::OnResponseRecived(Request, Response, bConnectedSuccessfully);
	if (!Response)
	{
		return;
	}
	FString Errors;
	FString Results;
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);
	if (ResponseObj.IsValid())
	{
		if (ResponseObj->TryGetStringField("errors", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}
		else if (ResponseObj->TryGetStringField("value", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}
		else if (ResponseObj->TryGetStringField("error", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}
		FString Ability1;
		FString Ability2;
		FString Ability3;
		if (ResponseObj->TryGetStringField("Ability1", Ability1) || ResponseObj->TryGetStringField("Ability2", Ability2) || ResponseObj->TryGetStringField("Ability3", Ability3))
		{
			OnBuildFinded.Broadcast(Ability1, Ability2, Ability3);
		}
		FString BuildSaveStatus;
		if (ResponseObj->TryGetStringField("BuildSaveInfo", BuildSaveStatus))
		{
			OnBuildSaved.Broadcast();
		}
	}

}