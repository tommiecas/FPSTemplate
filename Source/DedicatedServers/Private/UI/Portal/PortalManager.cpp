// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/API_Data.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/APITest/API_TestManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"

void UPortalManager::JoinGameSession()
{
	OnJoinGameSessionMessage.Broadcast(TEXT("Searching for Game Session..."), false);

	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::FindOrCreateGameSessionResponse);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameSessionsAPI::FindOrCreateGameSessionResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
}

void UPortalManager::FindOrCreateGameSessionResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("FindOrCreateGameSessionResponse"));

	if (!bWasSuccessful)
	{
		OnJoinGameSessionMessage.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true); 
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnJoinGameSessionMessage.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
		}
		DumpMetaData(JsonObject);

		FDS_GameSessionResponse GameSessionResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSessionResponse);
		GameSessionResponse.Dump();

		OnJoinGameSessionMessage.Broadcast(TEXT("Found Game Session!"), false);
	}
}
