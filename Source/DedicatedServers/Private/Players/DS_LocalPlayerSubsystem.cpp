// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/DS_LocalPlayerSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"

#include "UI/Portal/Portal_Interfaces/PortalManagement.h"


void UDS_LocalPlayerSubsystem::InitializeTokens(const ::FDS_AuthenticationResult& InAuthResult,
	TScriptInterface<IPortalManagement> PortalManagement)
{
	AuthenticationResult = InAuthResult;
	UE_LOG(LogDedicatedServers, Log, TEXT("Access Token: %s"), *AuthenticationResult.AccessToken);
	PortalManagementInterface = PortalManagement;
	SetRefreshTokenTimer();
}

void UDS_LocalPlayerSubsystem::UpdateTokens(const FString& AccessToken, const FString& IDToken)
{
	// Ignore a refresh response that arrives after local sign-out.
	if (!IsValid(PortalManagementInterface.GetObject())) return;

	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IDToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

void UDS_LocalPlayerSubsystem::SetRefreshTokenTimer()
{
	UWorld* World = GetWorld();
	if (IsValid(World) && IsValid(PortalManagementInterface.GetObject()))
	{
		FTimerDelegate RefreshDelegate;
		RefreshDelegate.BindLambda([this]()
		{
			PortalManagementInterface->RefreshTokens(AuthenticationResult.RefreshToken);
		});
		World->GetTimerManager().SetTimer(RefreshTimer, RefreshDelegate, TokenRefreshInterval, false);
	}
}

void UDS_LocalPlayerSubsystem::ClearSession()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RefreshTimer);
	}
	AuthenticationResult = FDS_AuthenticationResult();
	PortalManagementInterface = TScriptInterface<IPortalManagement>();
	UserName.Empty();
	Email.Empty();
}
