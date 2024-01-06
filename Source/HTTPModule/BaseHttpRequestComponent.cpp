// All rights reserved Daniil Grigoriev
#include "BaseHttpRequestComponent.h"
#include "JsonObjectConverter.h"
#include <iostream>
#include <stdlib.h>



// Sets default values for this component's properties
UBaseHttpRequestComponent::UBaseHttpRequestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBaseHttpRequestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBaseHttpRequestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBaseHttpRequestComponent::SendRequest(FString Url, TSharedRef<FJsonObject> RequestObj, FString Verb, FString HeaderName, FString HeaderValue)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();


	Request->OnProcessRequestComplete().BindUObject(this, &UBaseHttpRequestComponent::OnResponseRecived);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	//RequestObj->SetStringField("subject", Subject);

	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(HeaderName, HeaderValue);
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

void UBaseHttpRequestComponent::CheckConnection(FString Url)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->SetURL(Url);
	Request->SetVerb("GET");

	Request->OnProcessRequestComplete().BindUObject(this, &UBaseHttpRequestComponent::LostConnectionCheck);

	Request->ProcessRequest();
}

void UBaseHttpRequestComponent::LostConnectionCheck(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!bConnectedSuccessfully)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection lost"));
		if (OnLostConnection.IsBound())
		{
			OnLostConnection.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Connection Success"));
		if (OnConnectionSuccess.IsBound())
		{
			OnConnectionSuccess.Broadcast();
		}
	}
}

void UBaseHttpRequestComponent::OnResponseRecived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);


	TSharedPtr<FJsonObject> MyJson = MakeShareable(new FJsonObject);

	TMap<FString, TSharedPtr<FJsonValue>> JsonValues = MyJson->Values;






	if (ResponseObj->TryGetField("errors") != nullptr)
	{
		OnResponse.Broadcast(false, "errors");

	}
	else
	{
		OnResponse.Broadcast(true, "Sended");
	}




	UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
}

