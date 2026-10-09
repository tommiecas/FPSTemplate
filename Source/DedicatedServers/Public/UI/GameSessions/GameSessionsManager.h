// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTP_RequestManager.h"
#include "GameSessionsManager.generated.h"


/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UGameSessionsManager : public UHTTP_RequestManager
{
	GENERATED_BODY()

public:
	void JoinGameSession();
	
	

private:
	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString GetUniquePlayerID();
	void TrackGameSessionStatus(const FString& Status, const FString& SessionID);
	void TryCreatePlayerSession(const FString& PlayerID, const FString& GameSessionID);

	FTimerHandle CreateSessionTimer;
};
