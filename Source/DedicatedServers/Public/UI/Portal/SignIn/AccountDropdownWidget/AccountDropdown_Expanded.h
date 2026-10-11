// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AccountDropdown_Expanded.generated.h"

class UDS_LocalPlayerSubsystem;
class UPortalManager;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropdown_Expanded : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignOut;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SignOutButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor OriginalTextColor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor HoveredTextColor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;
	void SetSignOutButtonStyleTransparent();


	UFUNCTION()
	void OnHoveringOverSignOutButton();

	UFUNCTION()
	void OnHoveringOffOfSignOutButton();

	UFUNCTION()
	void OnSignOutButtonClicked();

private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UDS_LocalPlayerSubsystem* GetLocalPlayerSubsystem();
};
