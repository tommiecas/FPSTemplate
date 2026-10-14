// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "API_Test_RecordMatchStatsWidget.generated.h"

class UButton;
class UGameStatsManager;

/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAPI_Test_RecordMatchStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_RecordMatchStats;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameStatsManager> GameStatsManagerClass;

protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UGameStatsManager> GameStatsManager;

	UFUNCTION()
	void OnRecordMatchStatsButtonClicked();
	
};
