// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/API_Data.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "UI/Portal/PortalManager.h"

void UGameSessionsManager::JoinGameSession()
{
	OnAPI_StatusMessageLeft.Broadcast(TEXT("Searching for Game Session..."), false);

	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::FindOrCreateGameSession_Response);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameSessionsAPI::FindOrCreateGameSessionResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
}

void UGameSessionsManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true); 
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
		}

		FDS_GameSessionResponse GameSessionResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSessionResponse);

		const FString GameSessionID = GameSessionResponse.GameSessionId;
		const FString GameSessionStatus = GameSessionResponse.Status;
		TrackGameSessionStatus(GameSessionStatus, GameSessionID);
	}
}

void UGameSessionsManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true); 
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
		}

		FDS_PlayerSessionResponse PlayerSessionResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSessionResponse);

		PlayerSessionResponse.Dump();

		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			FInputModeGameOnly InputModeData;
			LocalPlayerController->SetInputMode(InputModeData);
			LocalPlayerController->SetShowMouseCursor(false);
		}
		
		const FString IpAndPort = PlayerSessionResponse.IpAddress + TEXT(":") + FString::FromInt(PlayerSessionResponse.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address);
	}
}

FString UGameSessionsManager::GetUniquePlayerID()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		APlayerState* LocalPlayerState = LocalPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(LocalPlayerState) && LocalPlayerState->GetUniqueId().IsValid())
		{
			return TEXT("Player_") + FString::FromInt(LocalPlayerState->GetUniqueID());
		}
	}
	return FString();
}

void UGameSessionsManager::TrackGameSessionStatus(const FString& Status, const FString& SessionID)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Found active Game Session! Creating new Player Session..."), false);
		TryCreatePlayerSession(GetUniquePlayerID(), SessionID);	
	}
	else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate CreateSessionDelegate;
		CreateSessionDelegate.BindUObject(this, &ThisClass::JoinGameSession);
		
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			LocalPlayerController->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionDelegate, 0.5f, false);
		}
	}
	else
	{
		OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
	}
}

void UGameSessionsManager::TryCreatePlayerSession(const FString& PlayerID, const FString& GameSessionID)
{
	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::CreatePlayerSession_Response);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameSessionsAPI::CreatePlayerSessionResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TMap<FString, FString> ContentParams = {
		{ TEXT("playerId"), PlayerID },
		{ TEXT("gameSessionId"), GameSessionID }
	};
	const FString Content = SerializeJsonContent(ContentParams);
	
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

