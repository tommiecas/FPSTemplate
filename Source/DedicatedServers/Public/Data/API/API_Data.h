// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/DedicatedServersGameplayTags.h"
#include "API_Data.generated.h"

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPI_Data : public UDataAsset
{
	GENERATED_BODY()

public:
	FString GetAPI_Endpoint(const FGameplayTag& APIEndpoint);

protected:
	// Name of this API - for labeling in the Data Asset, and isn't used by any codet
	UPROPERTY(EditDefaultsOnly)
	FString API_Name;

	UPROPERTY(EditDefaultsOnly)
	FString API_InvokeURL;

	UPROPERTY(EditDefaultsOnly)
	FString API_Stage;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> API_Resources;
};
