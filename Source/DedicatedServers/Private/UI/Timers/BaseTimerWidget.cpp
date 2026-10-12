// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Timers/BaseTimerWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/KismetStringLibrary.h"
#include "Players/DS_PlayerController.h"

void UBaseTimerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OwningPlayerController = Cast<ADS_PlayerController>(GetOwningPlayer());
	if (IsValid(OwningPlayerController))
	{
		OwningPlayerController->OnTimerUpdated.AddDynamic(this, &UBaseTimerWidget::OnTimerUpdated);
		OwningPlayerController->OnTimerStopped.AddDynamic(this, &UBaseTimerWidget::OnTimerStopped);
	}

	if (bHiddenWhenInactive)
	{
		TextBlock_Time->SetRenderOpacity(0.f);
	}
}

void UBaseTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bActive)
	{
		UpdateCountdown(InternalCountdown - InDeltaTime);
	}
}

void UBaseTimerWidget::OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type)
{
	if (Type != TimerType) return;

	if (!bActive)
	{
		TimerStarts(CountdownTimeLeft);
	}
	UpdateCountdown(CountdownTimeLeft);
	K2_OnTimerUpdated(CountdownTimeLeft, TimerType);


	
}

void UBaseTimerWidget::OnTimerStopped(float CountdownTimeLeft, ECountdownTimerType Type)
{
	if (Type != TimerType) return;
	TimerStops();
	K2_OnTimerStopped(0.f, TimerType);

}

FString UBaseTimerWidget::FormatTimeAsString(float TimeInSeconds) const
{
	TimeInSeconds = bCanBeNegative ? TimeInSeconds : FMath::Abs(TimeInSeconds);
	FString DisplayTimeString;
	if (bShowCentiSeconds)
	{
		DisplayTimeString = UKismetStringLibrary::TimeSecondsToString(TimeInSeconds);
	}
	else
	{
		const TCHAR* NegativeModifier = TimeInSeconds < 0.f? TEXT("-") : TEXT("");
		TimeInSeconds = FMath::Abs(TimeInSeconds);

		const int32 NumMinutes = FMath::FloorToInt(TimeInSeconds / 60.f);
		const int32 NumSeconds = FMath::FloorToInt(TimeInSeconds - (NumMinutes * 60.f));

		DisplayTimeString = FString::Printf(TEXT("%s%02d:%02d"), NegativeModifier, NumMinutes, NumSeconds);
	}
	return DisplayTimeString;	
}

void UBaseTimerWidget::TimerStarts(float InitialTime)
{
	bActive = true;
	TextBlock_Time->SetRenderOpacity(1.f);
	K2_OnTimerStarted(InitialTime, TimerType);
}

void UBaseTimerWidget::TimerStops()
{
	bActive = false;
	UpdateCountdown(0.f);
	if (bHiddenWhenInactive)
	{
		TextBlock_Time->SetRenderOpacity(0.f);
	}
}

void UBaseTimerWidget::UpdateCountdown(float TimeSeconds)
{
	InternalCountdown = TimeSeconds;
	const FText TimeText = FText::FromString(FormatTimeAsString(InternalCountdown));
	TextBlock_Time->SetText(TimeText);
}
