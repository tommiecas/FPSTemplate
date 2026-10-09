// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/API/GameSessions/JoinGameWidget.h"
#include "UI/Portal/PortalManager.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/USuccessConfirmedPage.h"

void USignInOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	check(PortalManagerClass);
	
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::OnSignInButtonClicked);
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	SignInPage->Button_Quit->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	PortalManager->SignInStatusMessageDelegate.AddDynamic(SignInPage, &USignInPage::UpdateAPI_SignInStatusMessage);

	SignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::OnSignUpButtonClicked);
	PortalManager->SignUpStatusMessageDelegate.AddDynamic(SignUpPage, &USignUpPage::UpdateAPI_SignUpStatusMessage);
	PortalManager->OnSignUpSucceeded.AddDynamic(this, &USignInOverlay::OnSignUpSucceeded);
	
	ConfirmSignUpPage->Button_Confirm->OnClicked.AddDynamic(this, &USignInOverlay::OnConfirmButtonClicked);
	ConfirmSignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->OnConfirmSucceeded.AddDynamic(this, &USignInOverlay::OnConfirmSucceeded);
	PortalManager->ConfirmStatusMessageDelegate.AddDynamic(ConfirmSignUpPage, &UConfirmSignUpPage::UpdateConfirmationStatusMessage);

	SuccessConfirmedPage->Button_OK->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
}

void USignInOverlay::ShowSignInPage()
{
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage()
{
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}

void USignInOverlay::ShowConfirmSignUpPage()
{
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}

void USignInOverlay::ShowSuccessConfirmedPage()
{
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage);
}

void USignInOverlay::OnSignInButtonClicked()
{
	const FString Username = SignInPage->TextBox_UserName->GetText().ToString();
	const FString Password = SignInPage->TextBox_Password->GetText().ToString();
	PortalManager->SignIn(Username, Password);
}

void USignInOverlay::OnSignUpButtonClicked()
{
	const FString Username = SignUpPage->TextBox_UserName->GetText().ToString();
	const FString Password = SignUpPage->TextBox_Password->GetText().ToString();
	const FString Email = SignUpPage->TextBox_Email->GetText().ToString();
	PortalManager->SignUp(Username, Password, Email);
}

void USignInOverlay::OnConfirmButtonClicked()
{
	const FString ConfirmationCode = ConfirmSignUpPage->TextBox_ConfirmationCode->GetText().ToString();
	ConfirmSignUpPage->Button_Confirm->SetIsEnabled(false);
	PortalManager->Confirm(ConfirmationCode);
}

void USignInOverlay::OnSignUpSucceeded()
{
	SignUpPage->ClearTextBoxWidgets();
	ConfirmSignUpPage->TextBlock_Destination->SetText(FText::FromString(PortalManager->LastSignUpResponse.CodeDeliveryDetails.Destination));
	ShowConfirmSignUpPage();
}

void USignInOverlay::OnConfirmSucceeded()
{
	ConfirmSignUpPage->ClearTextBoxWidgets();
	ShowSuccessConfirmedPage();
}
