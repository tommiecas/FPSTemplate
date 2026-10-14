// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameStats/GameStatsManager.h"

#include "JsonObjectConverter.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Data/API/API_Data.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpResponse.h"
#include "Players/DS_LocalPlayerSubsystem.h"

void UGameStatsManager::RecordMatchStats(const FDS_RecordMatchStatsInput& RecordMatchStatsInput)
{
	if (!IsValid(API_Data))
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats requires an API data asset assigned to '%s'."), *GetPathName());
		return;
	}
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameStatsAPI::RecordMatchStatsResource);
	if (API_URL.IsEmpty())
	{
		return;
	}
	
	FString JsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDS_RecordMatchStatsInput::StaticStruct(), &RecordMatchStatsInput, JsonString);
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RecordMatchStats_Response);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UGameStatsManager::RecordMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
												  bool bWasSuccessful)
{
	if (!bWasSuccessful) UE_LOG(LogDedicatedServers, Error, TEXT("RecordMatchStats failed."));

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject)) return;
	}
}

void UGameStatsManager::RetrieveMatchStats()
{
	RetrieveMatchStatsStatusMessage.Broadcast(TEXT("Retrieving Match Stats..."), false);
	
	UDS_LocalPlayerSubsystem* DS_LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem();
	if (!IsValid(DS_LocalPlayerSubsystem)) return;
	check(API_Data);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameStatsAPI::RetrieveMatchStatsResource);
	Request->OnProcessRequestComplete().BindUObject(this, &UGameStatsManager::RetrieveMatchStats_Response);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TMap<FString, FString> Params = {
		{ TEXT("accessToken"), DS_LocalPlayerSubsystem->GetFDS_AuthenticationResult().AccessToken }
	};
	const FString Content = SerializeJsonContent(Params);

	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UGameStatsManager::RetrieveMatchStats_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse());
		RetrieveMatchStatsStatusMessage.Broadcast(HTTP_StatusMessages::SomethingWentWrong, false);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnRetrieveMatchStatsResponseReceived.Broadcast(FDSRetrieveMatchStatsResponse());
			RetrieveMatchStatsStatusMessage.Broadcast(HTTP_StatusMessages::SomethingWentWrong, false);
			return;
		}

		FDSRetrieveMatchStatsResponse RetrieveMatchStats_Response;

		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &RetrieveMatchStats_Response);
		RetrieveMatchStats_Response.Dump();

		OnRetrieveMatchStatsResponseReceived.Broadcast(RetrieveMatchStats_Response);
		RetrieveMatchStatsStatusMessage.Broadcast(TEXT(""), false);
	}
}