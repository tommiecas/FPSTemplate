// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "API_TestHUD.generated.h"

class UAPI_TestOverlay;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API AAPI_TestHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPI_TestOverlay> API_TestOverlayClass;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAPI_TestOverlay> API_TestOverlay;
	
};
