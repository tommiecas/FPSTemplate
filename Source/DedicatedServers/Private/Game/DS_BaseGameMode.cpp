// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DedicatedServersTypes/DS_Types.h"
#include "Players/DS_PlayerController.h"
#include "GameLiftServerSDK.h"

void ADS_BaseGameMode::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this, [&]()
	{
		OnCountdownTimerFinished(CountdownTimerHandle.Type);
	});

	GetWorldTimerManager().SetTimer(CountdownTimerHandle.TimerFinishedHandle, CountdownTimerHandle.TimerFinishedDelegate, CountdownTimerHandle.CountdownTime, false);

	CountdownTimerHandle.TimerUpdateDelegate.BindWeakLambda(this, [&]()
	{
		UpdateCountdownTimer(CountdownTimerHandle);
	});

	GetWorldTimerManager().SetTimer(CountdownTimerHandle.TimerUpdateHandle, CountdownTimerHandle.TimerUpdateDelegate, CountdownTimerHandle.CountdownUpdateInterval, true);
	UpdateCountdownTimer(CountdownTimerHandle);
}

void ADS_BaseGameMode::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.State = ECountdownTimerState::Stopped;
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerFinishedHandle);

	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerUpdateHandle);
	if (CountdownTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountdownTimerHandle.TimerFinishedDelegate.Unbind();
	}
	if (CountdownTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountdownTimerHandle.TimerUpdateDelegate.Unbind();
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(Iterator->Get());
		if (IsValid(DS_PlayerController))
		{
			DS_PlayerController->Client_TimerStopped(0.f, CountdownTimerHandle.Type);
		}
	}
}

void ADS_BaseGameMode::UpdateCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(Iterator->Get());
		if (IsValid(DS_PlayerController))
		{
			const float CountdownTimeLeftOnClock = CountdownTimerHandle.CountdownTime - GetWorldTimerManager().GetTimerElapsed(CountdownTimerHandle.TimerFinishedHandle);
			DS_PlayerController->Client_TimerUpdated(CountdownTimeLeftOnClock, CountdownTimerHandle.Type);
		}
	}
}

void ADS_BaseGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	
}

void ADS_BaseGameMode::TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap)
{
	const FString MapName = DestinationMap.ToSoftObjectPath().GetAssetName();
	if (GIsEditor)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
	else
	{
		GetWorld()->ServerTravel(MapName);
	}
}

void ADS_BaseGameMode::RemovePlayerSession(AController* Exiting)
{
	ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(Exiting);
	if (!IsValid(DS_PlayerController)) return;

#if WITH_GAMELIFT
	const FString& PlayerSessionId = DS_PlayerController->PlayerSessionId;
	if (!PlayerSessionId.IsEmpty())
	{
		Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
	}
#endif
}
