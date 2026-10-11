// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/API_Data.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Players/DS_LocalPlayerSubsystem.h"
#include "UI/APITest/API_TestManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "UI/UI_Interfaces/HUDManagement.h"
#include "GameFramework/HUD.h"


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
		
		UDS_LocalPlayerSubsystem* DS_LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem();
		if (IsValid(DS_LocalPlayerSubsystem))
		{
			DS_LocalPlayerSubsystem->InitializeTokens(InitiateAuth_Response.AuthenticationResult, this);
		
			DS_LocalPlayerSubsystem->UserName = LastUsername;
			DS_LocalPlayerSubsystem->Email = InitiateAuth_Response.email;
		}
		APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(LocalPlayerController))
		{
			if (IHUDManagement* HUDManagementInterface = Cast<IHUDManagement>(LocalPlayerController->GetHUD()))
			{
				HUDManagementInterface->OnSignIn();
			}
		}
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

void UPortalManager::RefreshTokens(const FString& RefreshToken)
{
	check(API_Data);
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::RefreshTokens_Response);
	const FString API_Url = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::PortalAPI::SignInResource);
	Request->SetURL(API_Url);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TMap<FString, FString> Params = {
		{ TEXT("refreshToken"), RefreshToken }
	};
	const FString Content = SerializeJsonContent(Params);
	Request->SetContentAsString(Content);
	Request->ProcessRequest();
}

void UPortalManager::RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	{
		if (ContainsErrors(JsonObject)) return;
		
		FDS_InitiateAuth_Response InitiateAuth_Response;
		FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InitiateAuth_Response);

		UDS_LocalPlayerSubsystem* DS_LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem();
		if (IsValid(DS_LocalPlayerSubsystem))
		{
			DS_LocalPlayerSubsystem->UpdateTokens(InitiateAuth_Response.AuthenticationResult.AccessToken, InitiateAuth_Response.AuthenticationResult.IdToken);
		
		}
	}
}

void UPortalManager::SignOut(const FString& AccToken)
{
	// Local sign-out must not depend on the revocation endpoint being available.
	if (IsValid(API_Data) && !AccToken.IsEmpty())
	{
		TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
		// The widget and its manager may be collected after returning to SignIn.
		Request->OnProcessRequestComplete().BindStatic(&UPortalManager::SignOut_Response);
		const FString API_Url = API_Data->GetAPI_Endpoint(DedicatedServersGameplayTags::PortalAPI::SignOutResource);
		Request->SetURL(API_Url);
		Request->SetVerb(TEXT("POST"));
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

		TMap<FString, FString> Params = {
			{ TEXT("accessToken"), AccToken }
		};
		const FString Content = SerializeJsonContent(Params);
		Request->SetContentAsString(Content);
		if (!Request->ProcessRequest())
		{
			UE_LOG(LogTemp, Warning, TEXT("Sign Out: could not start server token revocation; signing out locally."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sign Out: missing API configuration or access token; signing out locally."));
	}

	if (UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetDS_LocalPlayerSubsystem())
	{
		LocalPlayerSubsystem->ClearSession();
	}
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		if (IHUDManagement* HUDManagementInterface = Cast<IHUDManagement>(LocalPlayerController->GetHUD()))
		{
			HUDManagementInterface->OnSignOut();
		}
	}
}

void UPortalManager::SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sign Out: server token revocation failed; local session is already cleared."));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid() &&
		(JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")) || JsonObject->HasField(TEXT("$fault"))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Sign Out: server rejected token revocation; local session is already cleared."));
	}
}

void UPortalManager::QuitGame()
{
	APlayerController* LocalPlayerController = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(LocalPlayerController))
	{
		UKismetSystemLibrary::QuitGame(this, LocalPlayerController, EQuitPreference::Quit, false);
	}
}











