// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "GameFramework/GameMode.h"
#include "DS_GameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDS_GameMode, Log, All);

struct FProcessParameters;
/**
 * 
 */
UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode : public AGameMode
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

private:
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void ParseCommandLinePort(int32& OutPort);
	
	FProcessParameters ProcessParameters;
};
