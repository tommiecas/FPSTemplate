// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/PlayerLabel.h"

#include "Components/TextBlock.h"

void UPlayerLabel::SetUserName(const FString& UserName) const
{
	TextBlock_UserName->SetText(FText::FromString(UserName));
}

FString UPlayerLabel::GetUserName() const
{
	return TextBlock_UserName->GetText().ToString();
}
