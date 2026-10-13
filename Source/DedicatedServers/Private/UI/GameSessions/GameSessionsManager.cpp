// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "aws/gamelift/server/model/PlayerSession.h"
#include "Data/API/API_Data.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "UI/Portal/PortalManager.h"
#include "Players/DS_LocalPlayerSubsystem.h"

void UGameSessionsManager::JoinGameSession()
{
	UDS_LocalPlayerSubsystem* SessionSubsystem = GetDS_LocalPlayerSubsystem();
	if (!IsValid(SessionSubsystem) || SessionSubsystem->GetFDS_AuthenticationResult().AccessToken.IsEmpty())
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Please sign in again before joining a game."), true);
		return;
	}
	OnAPI_StatusMessageLeft.Broadcast(TEXT("Searching for Game Session..."), false);

	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::FindOrCreateGameSession_Response);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameSessionsAPI::FindOrCreateGameSessionResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		Request->SetHeader(TEXT("Authorization"), LocalPlayerSubsystem->GetFDS_AuthenticationResult().AccessToken);
	}

	

	if (!Request->ProcessRequest())
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Could not start the game-session request."), true);
	}
}

void UGameSessionsManager::FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Could not reach the game-session service. Please try again."), true);
		return;
	}
	if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogTemp, Error, TEXT("FindOrCreateGameSession failed: HTTP %d"), Response->GetResponseCode());
		OnAPI_StatusMessageLeft.Broadcast(FString::Printf(TEXT("Game-session search failed (HTTP %d)."), Response->GetResponseCode()), true);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
			return;
		}

		FDS_GameSessionResponse GameSessionResponse;
		if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &GameSessionResponse) || GameSessionResponse.GameSessionId.IsEmpty())
		{
			OnAPI_StatusMessageLeft.Broadcast(TEXT("Game-session service returned an invalid session."), true);
			return;
		}

		const FString GameSessionID = GameSessionResponse.GameSessionId;
		const FString GameSessionStatus = GameSessionResponse.Status;
		TrackGameSessionStatus(GameSessionStatus, GameSessionID);
	}
	else
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Game-session service returned an unreadable response."), true);
	}
}

void UGameSessionsManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Could not reach the player-session service. Please try again."), true);
		return;
	}
	if (!EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogTemp, Error, TEXT("CreatePlayerSession failed: HTTP %d"), Response->GetResponseCode());
		OnAPI_StatusMessageLeft.Broadcast(FString::Printf(TEXT("Player-session creation failed (HTTP %d)."), Response->GetResponseCode()), true);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnAPI_StatusMessageLeft.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
			return;
		}

		FDS_PlayerSessionResponse PlayerSessionResponse;
		if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &PlayerSessionResponse) ||
			PlayerSessionResponse.IpAddress.IsEmpty() || PlayerSessionResponse.Port <= 0)
		{
			OnAPI_StatusMessageLeft.Broadcast(TEXT("Player-session service returned an invalid server address."), true);
			return;
		}

		PlayerSessionResponse.Dump();

		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			FInputModeGameOnly InputModeData;
			LocalPlayerController->SetInputMode(InputModeData);
			LocalPlayerController->SetShowMouseCursor(false);
		}

		const FString Options = "?PlayerSessionId=" + PlayerSessionResponse.PlayerSessionId + "?Username=" + PlayerSessionResponse.PlayerId;
		const FString IpAndPort = PlayerSessionResponse.IpAddress + TEXT(":") + FString::FromInt(PlayerSessionResponse.Port);
		const FName Address(*IpAndPort);
		UGameplayStatics::OpenLevel(this, Address, true, Options);
	}
	else
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Player-session service returned an unreadable response."), true);
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
	// The portal can run without an online-subsystem unique ID.
	return FString(TEXT("Player_")) + FGuid::NewGuid().ToString(EGuidFormats::Digits);
}

void UGameSessionsManager::TrackGameSessionStatus(const FString& Status, const FString& SessionID)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Found active Game Session! Creating new Player Session..."), false);
		if (UDS_LocalPlayerSubsystem* DS_LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem(); IsValid(DS_LocalPlayerSubsystem))
		{
			TryCreatePlayerSession(DS_LocalPlayerSubsystem->UserName, SessionID);
		}
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
		UE_LOG(LogTemp, Error, TEXT("Cannot join game session with status: %s"), *Status);
		OnAPI_StatusMessageLeft.Broadcast(FString::Printf(TEXT("Game session is not ready (%s). Please try again."), *Status), true);
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

	if (UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem())
	{
		Request->SetHeader(TEXT("Authorization"), LocalPlayerSubsystem->GetFDS_AuthenticationResult().AccessToken);
	}

	TMap<FString, FString> ContentParams = {
		{ TEXT("playerId"), PlayerID },
		{ TEXT("gameSessionId"), GameSessionID }
	};
	const FString Content = SerializeJsonContent(ContentParams);
	
	Request->SetContentAsString(Content);
	if (!Request->ProcessRequest())
	{
		OnAPI_StatusMessageLeft.Broadcast(TEXT("Could not start the player-session request."), true);
	}
}

