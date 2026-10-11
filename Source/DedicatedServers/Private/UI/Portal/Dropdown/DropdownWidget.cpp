// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portal/Dropdown/DropdownWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UDropdownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Expander->OnClicked.AddDynamic(this, &UDropdownWidget::ToggleDropdown);
	Button_Expander->OnHovered.AddDynamic(this, &UDropdownWidget::OnHoveringOverDropdown);
	Button_Expander->OnUnhovered.AddDynamic(this, &UDropdownWidget::OnMovingAwayFromDropdown);
}

void UDropdownWidget::SetStyleTransparent()
{
	FButtonStyle Style;
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(FLinearColor(0.f, 0.f, 0.f, 0.f));
	Style.Disabled = Brush;
	Style.Hovered = Brush;
	Style.Pressed = Brush;
	Style.Normal = Brush;
	Button_Expander->SetStyle(Style);
}

void UDropdownWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	SetStyleTransparent();
	CollapseDropdownWidget();
	bIsAlreadyExpanded = false;
	OnMovingAwayFromDropdown();
}

void UDropdownWidget::ToggleDropdown()
{
	if (bIsAlreadyExpanded)
	{
		CollapseDropdownWidget();
	}
	else
	{
		ExpandDropdownWidget();
	}
}

void UDropdownWidget::ExpandDropdownWidget()
{
	WidgetSwitcher->SetActiveWidget(ExpandedWidget);
	bIsAlreadyExpanded = true;
	Image_Triangle->SetBrush(Triangle_Up);
}

void UDropdownWidget::CollapseDropdownWidget()
{
	WidgetSwitcher->SetActiveWidget(CollapsedWidget);
	bIsAlreadyExpanded = false;
	Image_Triangle->SetBrush(Triangle_Down);
}

void UDropdownWidget::OnHoveringOverDropdown()
{
	TextBlock_ButtonText->SetColorAndOpacity(HoveredTextColor);
}

void UDropdownWidget::OnMovingAwayFromDropdown()
{
	TextBlock_ButtonText->SetColorAndOpacity(OriginalTextColor);
}
