// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/ConfirmSignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UConfirmSignUpPage::ClearTextBoxWidgets()
{
	TextBox_ConfirmationCode->SetText(FText::GetEmpty());
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
	TextBlock_Destination->SetText(FText::GetEmpty());
}

void UConfirmSignUpPage::UpdateConfirmationStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_Confirm->SetIsEnabled(true);
	}
}

void UConfirmSignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_ConfirmationCode->OnTextChanged.AddDynamic(this, &UConfirmSignUpPage::UpdateConfirmButtonState);
	Button_Confirm->SetIsEnabled(false);
}

void UConfirmSignUpPage::UpdateConfirmButtonState(const FText& Text)
{
	const FRegexPattern SixDigitsPattern(TEXT(R"(^\d{6}$)"));
	FRegexMatcher SixDigitsMatcher(SixDigitsPattern, Text.ToString());

	const bool bIsConfirmationCodeValid = SixDigitsMatcher.FindNext();
	Button_Confirm->SetIsEnabled(bIsConfirmationCodeValid);
	if (bIsConfirmationCodeValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(""));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter six numerical digits.")));
	}
}
