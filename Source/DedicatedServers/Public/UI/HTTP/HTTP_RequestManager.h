// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTP_RequestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPI_StatusMessage, const FString&, Message, bool, bShouldResetWidgets);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAPIRequestSucceeded);

class UAPI_Data;
class FJsonObject;

/**
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTP_RequestManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAPI_StatusMessage OnAPI_StatusMessageLeft;

protected:
	void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);
	bool ContainsErrors(TSharedPtr<FJsonObject> JsonObject);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPI_Data> API_Data;

	FString SerializeJsonContent(const TMap<FString, FString>& Params);
};
