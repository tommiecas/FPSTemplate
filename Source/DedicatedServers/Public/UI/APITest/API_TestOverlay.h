// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "API_TestOverlay.generated.h"

class UFleetID;
struct FDS_ListFleetsResponse;
class UAPI_TestManager;
class UListFleetsScrollBoxWidget;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPI_TestOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPI_TestManager> API_TestManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFleetID> FleetIDWidgetClass;
	
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetsScrollBoxWidget> ListFleetsScrollBoxWidget;
	
	UPROPERTY()
	TObjectPtr<UAPI_TestManager> API_TestManager;

	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse, bool bWasSuccessful);

	
};
