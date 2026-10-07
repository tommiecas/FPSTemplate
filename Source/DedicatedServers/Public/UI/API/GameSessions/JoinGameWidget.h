// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinGameWidget.generated.h"

class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UJoinGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetStatusMessage(const FString& Message) const;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_JoinGame;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
	
};
