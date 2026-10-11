// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DropdownWidget.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UDropdownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> ExpandedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> CollapsedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Expander;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor OriginalTextColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Triangle_Up;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush Triangle_Down;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Triangle;

protected:
	virtual void NativeConstruct() override;
	void SetStyleTransparent();
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void ToggleDropdown();

	bool bIsAlreadyExpanded = false;

	void ExpandDropdownWidget();
	void CollapseDropdownWidget();

	UFUNCTION()
	void OnHoveringOverDropdown();

	UFUNCTION()
	void OnMovingAwayFromDropdown();
};
