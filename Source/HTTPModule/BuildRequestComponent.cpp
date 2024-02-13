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
		int PlayerId;
		if (ResponseObj->TryGetStringField("Ability1", Ability1) && ResponseObj->TryGetStringField("Ability2", Ability2) && ResponseObj->TryGetStringField("Ability3", Ability3) && ResponseObj->TryGetNumberField("PlayerId", PlayerId))
		{
			OnBuildFinded.Broadcast(Ability1, Ability2, Ability3, PlayerId);
		}
		FString BuildSaveStatus;
		if (ResponseObj->TryGetStringField("BuildSaveInfo", BuildSaveStatus))
		{
			OnBuildSaved.Broadcast();
		}
		const TArray<TSharedPtr<FJsonValue>>* Items;
		if (ResponseObj->TryGetArrayField("ItemsId", Items))
		{
			ItemsId.Empty();
			for (TSharedPtr<FJsonValue> Value : *Items)
			{
				int Id;
				const TArray<TSharedPtr<FJsonValue>>* OutArray;
				
				if (Value->TryGetArray(OutArray))
				{
					for (TSharedPtr<FJsonValue> inValue: *OutArray)
					{
						if (inValue->TryGetNumber(Id))
						{
							ItemsId.Add(Id);
							OnOpenedItemsLoaded.Broadcast(Id);
						}
						
					}
					
				}
				
			}
			
		}

	}

}

void UBuildRequestComponent::GetAbilities(int PlayerId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("PlayerId", PlayerId);

	SendRequest(GetBuildUrl, RequestObj);
}

void UBuildRequestComponent::GetOpenedItems(int PlayerId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("Id", PlayerId);

	SendRequest(GetItemsUrl, RequestObj);
}

void UBuildRequestComponent::SaveAbilities(int PlayerId, FString Ability1, FString Ability2, FString Ability3, FString SessionId)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetNumberField("PlayerId", PlayerId);
	RequestObj->SetStringField("Ability1", Ability1);
	RequestObj->SetStringField("Ability2", Ability2);
	RequestObj->SetStringField("Ability3", Ability3);
	RequestObj->SetStringField("SessionId", SessionId);

	SendRequest(SaveBuildUrl, RequestObj);
}
