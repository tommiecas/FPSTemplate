// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/API_TestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/API/ListFleets/FleetID.h"
#include "UI/API/ListFleets/ListFleetsScrollBoxWidget.h"
#include "UI/APITest/API_TestManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"

void UAPI_TestOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	check(API_TestManagerClass);
	API_TestManager = NewObject<UAPI_TestManager>(this, API_TestManagerClass);

	check(ListFleetsScrollBoxWidget);
	check(ListFleetsScrollBoxWidget->Button_ListFleets);
	ListFleetsScrollBoxWidget->Button_ListFleets->OnClicked.AddUniqueDynamic(this, &UAPI_TestOverlay::ListFleetsButtonClicked);
	
}

void UAPI_TestOverlay::ListFleetsButtonClicked()
{
	check(API_TestManager);
	API_TestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPI_TestOverlay::OnListFleetsResponseReceived);
	API_TestManager->ListFleets();
	ListFleetsScrollBoxWidget->Button_ListFleets->SetIsEnabled(false);
}

void UAPI_TestOverlay::OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse,
	bool bWasSuccessful)
{
	if (API_TestManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UAPI_TestOverlay::OnListFleetsResponseReceived))
	{
		API_TestManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UAPI_TestOverlay::OnListFleetsResponseReceived);
	}

	ListFleetsScrollBoxWidget->ScrollBox_ListFleets->ClearChildren();

	if (bWasSuccessful)
	{
		for (const FString& FleetID : ListFleetsResponse.FleetIds)
		{
			UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, FleetIDWidgetClass);
			FleetIDWidget->TextBlock_FleetID->SetText(FText::FromString(FleetID));
			ListFleetsScrollBoxWidget->ScrollBox_ListFleets->AddChild(FleetIDWidget);
		}
	}
	else
	{
		UFleetID* FleetIDWidget = CreateWidget<UFleetID>(this, FleetIDWidgetClass);
		FleetIDWidget->TextBlock_FleetID->SetText(FText::FromString("Something went wrong!"));
		ListFleetsScrollBoxWidget->ScrollBox_ListFleets->AddChild(FleetIDWidget);
	}

	ListFleetsScrollBoxWidget->Button_ListFleets->SetIsEnabled(true);
}



