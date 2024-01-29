// All rights reserved Daniil Grigoriev


#include "PlayerRequestComponent.h"
#include "Misc/SecureHash.h"

void UPlayerRequestComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response)
	{
		return;
	}
	//Super::OnResponseRecived(Request, Response, bConnectedSuccessfully);
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
		if (ResponseObj->TryGetStringField("error", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}
		else if (ResponseObj->TryGetStringField("value", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}

	}
}


void UPlayerRequestComponent::ChangePlayerStatus(EPlayerStatus NewStatus, int PlayerId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("Id", PlayerId);
	RequestObj->SetNumberField("NewStatus", static_cast<int>(NewStatus));

	SendRequest(ChangePlayerStatusUrl, RequestObj);
}


