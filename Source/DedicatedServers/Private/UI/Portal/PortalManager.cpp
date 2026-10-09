// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "ISourceControlProvider.h"
#include "JsonObjectConverter.h"
#include "aws/gamelift/server/model/GameSession.h"
#include "aws/gamelift/server/model/PlayerSession.h"
#include "Components/SlateWrapperTypes.h"
#include "Data/API/API_Data.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "UI/APITest/API_TestManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"


void UPortalManager::SignIn(const FString& UserName, const FString& Password)
{
	SignInStatusMessageDelegate.Broadcast(TEXT("Signing in..."), false);
	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignIn_Response);
	const FString API_Url = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::PortalAPI::SignInResource);
	Request->SetURL(API_Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	LastUsername = UserName;
	TMap<FString, FString> Params = {
		{ TEXT("username"), UserName },
		{ TEXT("password"), Password }
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignInStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			SignInStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
			return;
		}

		FDS_InitiateAuth_Response InitiateAuth_Response;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InitiateAuth_Response);
		InitiateAuth_Response.Dump();
	}
}

void UPortalManager::SignUp(const FString& UserName, const FString& Password, const FString& Email)
{
	SignUpStatusMessageDelegate.Broadcast(TEXT("Creating a new account..."), false);
	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignUp_Response);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::PortalAPI::SignUpResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	LastUsername = UserName;
	TMap<FString, FString> ContentParams = {
		{ TEXT("username"), UserName },
		{ TEXT("password"), Password },
		{ TEXT("email"), Email }
	};

	const FString Content = SerializeJsonContent(ContentParams);
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UPortalManager::SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignUpStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			SignUpStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
			return;
		}

		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LastSignUpResponse);

		OnSignUpSucceeded.Broadcast();
	}
}

void UPortalManager::Confirm(const FString& ConfirmationCode)
{
	check(API_Data);
	ConfirmStatusMessageDelegate.Broadcast(TEXT("Checking confirmation code..."), false);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::Confirm_Response);
	const FString API_URL = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::PortalAPI::ConfirmSignUpResource);
	Request->SetURL(API_URL);
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	
	TMap<FString, FString> ContentParams = {
		{ TEXT("username"), LastUsername },
		{ TEXT("confirmationCode"), ConfirmationCode }
	};

	const FString Content = SerializeJsonContent(ContentParams);
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UPortalManager::Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ConfirmStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject))
		{
			if (JsonObject->HasField(TEXT("name")))
			{
				FString Exception = JsonObject->GetStringField(TEXT("name"));
				if (Exception.Equals(TEXT("CodeMismatchException")))
				{
					ConfirmStatusMessageDelegate.Broadcast(TEXT("Incorrect Confirmation Code"), true);
					return;
				}
			}	
			ConfirmStatusMessageDelegate.Broadcast(HTTP_StatusMessages::SomethingWentWrong, true);
			return;
		}
	}
	OnConfirmSucceeded.Broadcast();
}

void UPortalManager::QuitGame()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}











