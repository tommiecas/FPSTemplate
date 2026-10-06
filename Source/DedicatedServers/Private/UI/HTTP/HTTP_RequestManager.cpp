// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HTTP/HTTP_RequestManager.h"

#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "UI/HTTP/HTTP_RequestTypes.h"

void UHTTP_RequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> JsonObjectMetaData = JsonObject->GetObjectField(TEXT("$metadata"));
		FDS_MetaData DS_MetaData;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObjectMetaData.ToSharedRef(), &DS_MetaData);

		DS_MetaData.Dump();
	}
}

bool UHTTP_RequestManager::ContainsErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown Error");
		FString ErrorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown Error Message");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Message: %s"), *ErrorMessage);

		return true;			
	}
	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString ErrorType = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown Error");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *ErrorType);
		return true;
	}
	else return false;
}
