// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTP_RequestTypes.h"
#include "UI/Portal/Portal_Interfaces/PortalManagement.h"
#include "DS_LocalPlayerSubsystem.generated.h"

class IPortalManagement;
struct FDS_AuthenticationResult;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDS_LocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	void InitializeTokens(const FDS_AuthenticationResult& InAuthResult, TScriptInterface<IPortalManagement> PortalManagement);
	void UpdateTokens(const FString& AccessToken, const FString& IDToken);
	void SetRefreshTokenTimer();
	void ClearSession();

	FString UserName;
	FString Email;
	
private:
	UPROPERTY()
	FDS_AuthenticationResult AuthenticationResult;

	UPROPERTY()
	TScriptInterface<IPortalManagement> PortalManagementInterface;

	float TokenRefreshInterval = 2700.f; // every 45 minutes
	FTimerHandle RefreshTimer;

public:
	FORCEINLINE FDS_AuthenticationResult GetFDS_AuthenticationResult() const { return AuthenticationResult; }

	
};
