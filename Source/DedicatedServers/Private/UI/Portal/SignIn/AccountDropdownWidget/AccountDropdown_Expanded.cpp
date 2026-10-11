// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/SignIn/AccountDropdownWidget/AccountDropdown_Expanded.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Players/DS_LocalPlayerSubsystem.h"
#include "UI/Portal/PortalManager.h"

void UAccountDropdown_Expanded::NativeConstruct()
{
	Super::NativeConstruct();

	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);

	Button_SignOut->OnClicked.AddDynamic(this, &UAccountDropdown_Expanded::OnSignOutButtonClicked);
	Button_SignOut->OnHovered.AddDynamic(this, &UAccountDropdown_Expanded::OnHoveringOverSignOutButton);
	Button_SignOut->OnUnhovered.AddDynamic(this, &UAccountDropdown_Expanded::OnHoveringOffOfSignOutButton);

	UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		TextBlock_Email->SetText(FText::FromString(LocalPlayerSubsystem->Email));
	}
}

void UAccountDropdown_Expanded::NativePreConstruct()
{
	Super::NativePreConstruct();

	OnHoveringOffOfSignOutButton();
	SetSignOutButtonStyleTransparent();
}

void UAccountDropdown_Expanded::SetSignOutButtonStyleTransparent()
{
	FButtonStyle Style;
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;
	Button_SignOut->SetStyle(Style);
}

void UAccountDropdown_Expanded::OnHoveringOverSignOutButton()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UAccountDropdown_Expanded::OnHoveringOffOfSignOutButton()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(OriginalTextColor);
}

void UAccountDropdown_Expanded::OnSignOutButtonClicked()
{
	Button_SignOut->SetIsEnabled(false);

	check(PortalManager);
	UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = GetLocalPlayerSubsystem();
	if (IsValid(LocalPlayerSubsystem))
	{
		FDS_AuthenticationResult AuthResult = LocalPlayerSubsystem->GetFDS_AuthenticationResult();
		PortalManager->SignOut(AuthResult.AccessToken);
	}
	else
	{
		PortalManager->SignOut(FString());
	}
}

UDS_LocalPlayerSubsystem* UAccountDropdown_Expanded::GetLocalPlayerSubsystem()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (IsValid(PlayerController) && IsValid(PlayerController->GetLocalPlayer()))
	{
		UDS_LocalPlayerSubsystem* LocalPlayerSubsystem = PlayerController->GetLocalPlayer()->GetSubsystem<UDS_LocalPlayerSubsystem>();
		if (IsValid(LocalPlayerSubsystem)) return LocalPlayerSubsystem;
	}
	return nullptr;
}
