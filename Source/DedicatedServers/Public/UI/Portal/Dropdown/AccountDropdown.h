// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropdownWidget.h"
#include "AccountDropdown.generated.h"

class UDS_LocalPlayerSubsystem;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API UAccountDropdown : public UDropdownWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UDS_LocalPlayerSubsystem* GetLocalPlayerSubsystem() const;
};
