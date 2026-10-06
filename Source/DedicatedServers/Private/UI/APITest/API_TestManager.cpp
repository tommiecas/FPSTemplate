// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/API_TestManager.h"

#include "Interfaces/IHttpRequest.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/API_Data.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "UI/HTTP/HTTP_RequestTypes.h"

void UAPI_TestManager::ListFleets()
{
	check(API_Data);
	
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UAPI_TestManager::ListFleets_Response);

	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::GameSessionsAPI::ListFleetsResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->ProcessRequest();
}

void UAPI_TestManager::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "List Fleets Response Received");

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			OnListFleetsResponseReceived.Broadcast(FDS_ListFleetsResponse(), false);
			return;
		}
		DumpMetaData(JsonObject);

		FDS_ListFleetsResponse DS_ListFleetsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &DS_ListFleetsResponse);
		DS_ListFleetsResponse.Dump();

		OnListFleetsResponseReceived.Broadcast(DS_ListFleetsResponse, true);

	}
}


	
	
