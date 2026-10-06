// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/APITest/API_TestHUD.h"

#include "UI/APITest/API_TestOverlay.h"

void AAPI_TestHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetOwningPlayerController();
	if (IsValid(PlayerController) && API_TestOverlayClass)
	{
		API_TestOverlay = CreateWidget<UAPI_TestOverlay>(PlayerController, API_TestOverlayClass);
		API_TestOverlay->AddToViewport();
	}
}
