// Fill out your copyright notice in the Description page of Project Settings.


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

	if (ResponseObj->TryGetStringField("errors", Errors))
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
}

void UAuthorizationComponent::RegistrationRequest(FString Password, FString Login, FString EMAIL)
{
	FString Url = "";
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();



	RequestObj->SetStringField("password", FMD5::HashAnsiString(*Password));
	RequestObj->SetStringField("login", Login);
	RequestObj->SetStringField("EMAIL", EMAIL);

	LastEMAIL = EMAIL;

	SendRequest(Url, RequestObj);
}

void UAuthorizationComponent::EmailConfirmRequest(FString Code)
{
	FString Url = "";
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();

	RequestObj->SetStringField("email", LastEMAIL);
	RequestObj->SetStringField("code", Code);

	SendRequest(Url, RequestObj);
}


