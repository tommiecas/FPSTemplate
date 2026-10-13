// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DS_BaseGameMode.h"
#include "DedicatedServersTypes/DS_Types.h"
#include "DS_MatchGameMode.generated.h"

enum class EMatchStatus : uint8;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_BaseGameMode
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	
	UPROPERTY(BlueprintReadOnly)
	EMatchStatus MatchStatus;

protected:
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap;

	void SetClientInputEnabled(bool bEnabled);
};
