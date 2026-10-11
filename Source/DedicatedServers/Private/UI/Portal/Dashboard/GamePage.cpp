// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dashboard/GamePage.h"

#include "Components/Button.h"
#include "UI/API/GameSessions/JoinGameWidget.h"
#include "UI/GameSessions/GameSessionsManager.h"

void UGamePage::NativeConstruct()
{
	Super::NativeConstruct();

	GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	GameSessionsManager->OnAPI_StatusMessageLeft.AddDynamic(JoinGameWidget, &UJoinGameWidget::SetStatusMessage);

	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &UGamePage::OnJoinGameButtonClicked);
}

void UGamePage::OnJoinGameButtonClicked()
{
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
	GameSessionsManager->JoinGameSession();
}
