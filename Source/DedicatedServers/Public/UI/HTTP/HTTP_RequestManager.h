// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTP_RequestManager.generated.h"

class UAPI_Data;
class FJsonObject;

/**
 * 
 */
UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTP_RequestManager : public UObject
{
	GENERATED_BODY()

protected:
	void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);
	bool ContainsErrors(TSharedPtr<FJsonObject> JsonObject);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPI_Data> API_Data;
};
