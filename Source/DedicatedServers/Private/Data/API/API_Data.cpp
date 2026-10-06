// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/API/API_Data.h"

FString UAPI_Data::GetAPI_Endpoint(const FGameplayTag& API_Endpoint)
{
	FString ResourceName = API_Resources.FindChecked(API_Endpoint);
	return API_InvokeURL + "/" + API_Stage + "/" + ResourceName;
}
