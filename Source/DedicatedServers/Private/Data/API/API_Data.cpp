// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/API_Data.h"
#include "DedicatedServers/DedicatedServers.h"

FString UAPI_Data::GetAPI_Endpoint(const FGameplayTag& API_Endpoint)
{
	const FString* ResourceName = API_Resources.Find(API_Endpoint);
	if (!ResourceName || ResourceName->IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("API data asset '%s' has no resource configured for tag '%s'. Check API_Resources."), *GetPathName(), *API_Endpoint.ToString());
		return FString();
	}
	return API_InvokeURL + "/" + API_Stage + "/" + *ResourceName;
}
