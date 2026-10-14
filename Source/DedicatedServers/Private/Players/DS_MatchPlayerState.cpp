// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/DS_MatchPlayerState.h"

#include "UI/GameStats/GameStatsManager.h"

void ADS_MatchPlayerState::OnMatchEnded(const FString& Username)
{
	
}

void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
}

void ADS_MatchPlayerState::RecordMatchStats(const FDS_RecordMatchStatsInput& RecordMatchStatsInput) const
{
	check(IsValid(GameStatsManager));
	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}
