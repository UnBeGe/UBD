// All rights reserved Daniil Grigoriev


#include "ServerRequestComponent.h"
#include "Misc/SecureHash.h"

void UServerRequestComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
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

		FString CheckPlayerResult;
		if (ResponseObj->TryGetStringField("Host", CheckPlayerResult))
		{
			OnCheckPlayerResult.Broadcast(true);
		}
	}
}

void UServerRequestComponent::IsPlayerAllowed(int PlayerId, int ServerId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("PlayerId", PlayerId);
	RequestObj->SetNumberField("ServerId", ServerId);

	SendRequest(CheckPlayerUrl, RequestObj);
}

void UServerRequestComponent::ChangeServerStatus(EServerStatus NewStatus, int ServerId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("ServerId", ServerId);
	RequestObj->SetNumberField("NewStatus", static_cast<int>(NewStatus));

	SendRequest(ChangeServerStatusUrl, RequestObj, "GET");
}


