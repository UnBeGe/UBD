// All rights reserved Daniil Grigoriev


#include "MatchMakingComponent.h"
#include "Misc/SecureHash.h"

void UMatchMakingComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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

		FString Server;
		if (ResponseObj->TryGetStringField("Host", Server))
		{
			OnServerFinded.Broadcast(Server);
		}
	}
}

void UMatchMakingComponent::FindServer(int Code, FString SessionId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("Id", Code);

	RequestObj->SetStringField("SessionId", SessionId);

	SendRequest(FindServerUrl, RequestObj);
}


