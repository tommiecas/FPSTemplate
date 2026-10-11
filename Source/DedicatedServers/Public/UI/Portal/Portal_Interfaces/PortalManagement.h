// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PortalManagement.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPortalManagement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVERS_API IPortalManagement
{
	GENERATED_BODY()

public:
	virtual void RefreshTokens(const FString& RefreshToken) = 0;
};
