// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTP_RequestManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "PortalManager.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTP_RequestManager
{
	GENERATED_BODY()

public:
	void SignIn(const FString& UserName, const FString& Password);
	void SignUp(const FString& UserName, const FString& Password, const FString& Email);
	void Confirm(const FString& ConfirmationCode);

	UFUNCTION()
	void QuitGame();

	UPROPERTY(BlueprintAssignable)
	FAPI_StatusMessage SignUpStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPI_StatusMessage ConfirmStatusMessageDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPI_StatusMessage SignInStatusMessageDelegate;

	UPROPERTY()
	FOnAPIRequestSucceeded OnSignUpSucceeded;

	UPROPERTY()
	FOnAPIRequestSucceeded OnConfirmSucceeded;

	FDS_SignUp_Response LastSignUpResponse;

	FString LastUsername;
	
private:
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Confirm_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
