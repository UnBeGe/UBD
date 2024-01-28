// All rights reserved Daniil Grigoriev


#include "ServerRequestComponent.h"
#include "Misc/SecureHash.h"

void UServerRequestComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
		bool Allowed = false;
		int PlayerID = -1;
		
		if (ResponseObj->TryGetNumberField("PlayerID", PlayerID))
		{
			FString Ban;
			FString Ability1;
			FString Ability2;
			FString Ability3;
			bool Build = ResponseObj->TryGetStringField("Ability1", Ability1) && ResponseObj->TryGetStringField("Ability2", Ability2) && ResponseObj->TryGetStringField("Ability3", Ability3);
			if (ResponseObj->TryGetStringField("Ban", Ban))
			{
				OnCheckPlayerResult.Broadcast(false, PlayerID, Ability1, Ability2, Ability3);
			}
			else
			{
				OnCheckPlayerResult.Broadcast(true, PlayerID, Ability1, Ability2, Ability3);
			}
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


