// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTP_RequestManager.h"
#include "API_TestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseReceived, const FDS_ListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPI_TestManager : public UHTTP_RequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleets();

	UPROPERTY()
	FOnListFleetsResponseReceived OnListFleetsResponseReceived;

private:
	void ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
