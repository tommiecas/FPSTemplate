// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/API_Test_RecordMatchStatsWidget.h"

#include "Components/Button.h"
#include "UI/GameStats/GameStatsManager.h"
#include "UI/HTTP/HTTP_RequestTypes.h"

void UAPI_Test_RecordMatchStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameStatsManager = NewObject<UGameStatsManager>(this, GameStatsManagerClass);
	Button_RecordMatchStats->OnClicked.AddDynamic(this, &UAPI_Test_RecordMatchStatsWidget::OnRecordMatchStatsButtonClicked);
}

void UAPI_Test_RecordMatchStatsWidget::OnRecordMatchStatsButtonClicked()
{
	check(GameStatsManager);

	const FString Username = TEXT("TestUser");
	FDS_RecordMatchStatsInput RecordMatchStatsInput;
	RecordMatchStatsInput.username = Username;
	RecordMatchStatsInput.matchStats.hits = 1;
	RecordMatchStatsInput.matchStats.headShotEliminations = 5;

	GameStatsManager->RecordMatchStats(RecordMatchStatsInput);
}
