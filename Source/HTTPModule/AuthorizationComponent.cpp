// All rights reserved Daniil Grigoriev


#include "AuthorizationComponent.h"
#include "Misc/SecureHash.h"

void UAuthorizationComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
		else if (ResponseObj->TryGetStringField("value", Errors))
		{
			OnResponse.Broadcast(false, Errors);
		}
		if (ResponseObj->TryGetStringField("ConfirmEmail", Results))
		{
			OnConfirmEmail.Broadcast();
		}
		if (ResponseObj->TryGetStringField("EmailConfirmed", Results))
		{
			OnEmailConfirmed.Broadcast();
		}
		FString Login;
		FString Id;
		if (ResponseObj->TryGetStringField("Login", Login) && ResponseObj->TryGetStringField("Id", Id))
		{
			OnAuth.Broadcast(Login, Id);
		}
	}
	
}

void UAuthorizationComponent::RegistrationRequest(FString Password, FString Login, FString EMAIL)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();



	RequestObj->SetNumberField("id", 0);
	RequestObj->SetStringField("login", Login);
	RequestObj->SetStringField("password", FMD5::HashAnsiString(*Password));
	RequestObj->SetStringField("email", EMAIL);
	RequestObj->SetNumberField("code", 0);
	RequestObj->SetBoolField("isActive", false);

	LastEMAIL = EMAIL;

	SendRequest(RegisterUrl, RequestObj);
}

void UAuthorizationComponent::EmailConfirmRequest(FString Code)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("email", LastEMAIL);
	RequestObj->SetStringField("code", Code);

	SendRequest(ConfirmUrl, RequestObj);
}

void UAuthorizationComponent::AuthRequest(FString Email, FString Password)
{
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("email", Email);
	RequestObj->SetStringField("password", FMD5::HashAnsiString(*Password));

	SendRequest(AuthUrl, RequestObj);
}


