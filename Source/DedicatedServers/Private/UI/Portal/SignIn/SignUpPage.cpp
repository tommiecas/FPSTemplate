// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void USignUpPage::UpdateAPI_SignUpStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_SignUp->SetIsEnabled(true);
	}
}

void USignUpPage::ClearTextBoxWidgets()
{
	TextBox_UserName->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	TextBox_ConfirmPassword->SetText(FText::GetEmpty());
	TextBox_Email->SetText(FText::GetEmpty());
}

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	Button_SignUp->SetIsEnabled(false);
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	const bool bIsUserNameValid = !TextBox_UserName->GetText().ToString().IsEmpty();
	const bool bArePasswordsMatching = TextBox_Password->GetText().ToString() == TextBox_ConfirmPassword->GetText().ToString();
	const bool bIsEmailValid = IsValidEmail(TextBox_Email->GetText().ToString());
	const bool bIsPasswordLongEnough = TextBox_Password->GetText().ToString().Len() >= 8;
	
	FString StatusMessage;
	const bool bIsStrongPassword = IsStrongPassword(TextBox_Password->GetText().ToString(), StatusMessage);
	if (!bIsStrongPassword)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	}
	else if (!bIsUserNameValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Username.")));
	}
	else if (!bArePasswordsMatching)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Passwords do not match.")));
	}
	else if (!bIsEmailValid)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter a valid Email.")));
	}
	else if (!bIsPasswordLongEnough)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Password must contain at least eight characters.")));
	}
	else
	{
		TextBlock_StatusMessage->SetText(FText::GetEmpty());
	}
	Button_SignUp->SetIsEnabled(bIsUserNameValid && bArePasswordsMatching && bIsEmailValid && bIsStrongPassword && bIsPasswordLongEnough);
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern EmailPattern(TEXT(R"((^[^\s@]+@[^\s@]+\.[^\s@]{2,}$))"));
	FRegexMatcher EmailMatcher(EmailPattern, Email);
	return EmailMatcher.FindNext();
}

bool USignUpPage::IsStrongPassword(const FString& Password, FString& StatusMessage)
{
	const FRegexPattern NumberPattern(TEXT(R"(\d)")); // contains at least one number
	const FRegexPattern SpecialCharPattern(TEXT(R"([^\w\s])")); // contains at least one special character
	const FRegexPattern UpperCasePattern(TEXT(R"([A-Z])")); // contains at least one uppercase letter
	const FRegexPattern LowerCasePattern(TEXT(R"([a-z])")); // contains at least one lowercase letter
	const FRegexPattern LengthPattern(TEXT(R"([^\s]{8,})")); // at least 8 characters long

	FRegexMatcher NumberMatcher(NumberPattern, Password);
	FRegexMatcher SpecialCharMatcher(SpecialCharPattern, Password);
	FRegexMatcher UpperCaseMatcher(UpperCasePattern, Password);
	FRegexMatcher LowerCaseMatcher(LowerCasePattern, Password);
	FRegexMatcher LengthMatcher(LengthPattern, Password);

	if (!NumberMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one number.");
		return false;
	}
	if (!SpecialCharMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one special character.");
		return false;
	}
	if (!UpperCaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one uppercase character.");
		return false;
	}
	if (!LowerCaseMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least one lowercase character.");
		return false;
	}
	if (!LengthMatcher.FindNext())
	{
		StatusMessage = TEXT("Password must contain at least eight characters.");
		return false;
	}
	return true;	
}
