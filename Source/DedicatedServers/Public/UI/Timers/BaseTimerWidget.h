// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DedicatedServersTypes/DS_Types.h"
#include "BaseTimerWidget.generated.h"

class UTextBlock;
class ADS_PlayerController;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UBaseTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	ECountdownTimerType TimerType = ECountdownTimerType::None;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADS_PlayerController> OwningPlayerController;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Updated"))
	void K2_OnTimerUpdated(float Time, ECountdownTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Stopped"))
	void K2_OnTimerStopped(float Time, ECountdownTimerType Type);
	
	UFUNCTION()
	virtual void OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type);

	UFUNCTION()
	virtual void OnTimerStopped(float CountdownTimeLeft, ECountdownTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Timer Started"))
	void K2_OnTimerStarted(float Time, ECountdownTimerType Type);
	
	UPROPERTY(BlueprintReadOnly)
	bool bActive = false;

	UPROPERTY(EditDefaultsOnly)
	bool bCanBeNegative = false;

	UPROPERTY(EditDefaultsOnly)
	bool bShowCentiSeconds = false;

	UPROPERTY(EditDefaultsOnly)
	bool bHiddenWhenInactive = false;

	FString FormatTimeAsString(float TimeInSeconds) const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time;

	float InternalCountdown;

	void TimerStarts(float InitialTime);
	void TimerStops();
	void UpdateCountdown(float TimeSeconds);
};
