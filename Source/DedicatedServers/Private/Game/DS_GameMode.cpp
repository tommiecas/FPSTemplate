// Fill out your copyright notice in the Description page of Project Settings.


#include "DedicatedServers/Public/Game/DS_GameMode.h"


#include "UObject/ConstructorHelpers.h"


#if WITH_GAMELIFT
#include "GameLiftServerSDK.h"
#include "GameLiftServerSDKModels.h"
#endif

DEFINE_LOG_CATEGORY(LogDS_GameMode);


void ADS_GameMode::BeginPlay()
{
	Super::BeginPlay();

#if WITH_GAMELIFT
	InitGameLift();
#endif
}

void ADS_GameMode::InitGameLift()
{
	UE_LOG(LogDS_GameMode, Log, TEXT("Initializing GameLift Server..."));
    
    FGameLiftServerSDKModule* GameLiftSDKModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));

    //Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
    FServerParameters ServerParametersForAnywhere;
    SetServerParameters(ServerParametersForAnywhere);
	GameLiftSDKModule->InitSDK(ServerParametersForAnywhere);

	auto OnGameSession = [=](Aws::GameLift::Server::Model::GameSession InGameSession)
	{
		FString GameSessionId = FString(InGameSession.GetGameSessionId());
		UE_LOG(LogDS_GameMode, Log, TEXT("GameSession Initializing: %s"), *GameSessionId);
		GameLiftSDKModule->ActivateGameSession();
	};

	ProcessParameters.OnStartGameSession.BindLambda(OnGameSession);

	auto OnProcessTerminate = [=]()
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Game Server Process is terminating."));
		GameLiftSDKModule->ProcessEnding();
	};

	ProcessParameters.OnTerminate.BindLambda(OnProcessTerminate);

	auto OnHealthCheck = []()
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Performing Health Check"));
		return true;
	};

	ProcessParameters.OnHealthCheck.BindLambda(OnHealthCheck);

	//GameServer.exe -port=7777 LOG=server.mylog
	int32 StandardPort = FURL::UrlConfig.DefaultPort;
	if (StandardPort < 1 || StandardPort > 65535)
	{
		StandardPort = 7777;
	}
	ParseCommandLinePort(StandardPort);
	ProcessParameters.port = StandardPort;

	//Here, the game server tells Amazon GameLift Servers where to find game session log files.
	//At the end of a game session, Amazon GameLift Servers uploads everything in the specified 
	//location and stores it in the cloud for access later.
	TArray<FString> Logfiles;
	Logfiles.Add(TEXT("FPSTemplate/Saved/Logs/FPSTemplate.log"));
	ProcessParameters.logParameters = Logfiles;

    UE_LOG(LogDS_GameMode, Log, TEXT("Calling Process Ready."));
	GameLiftSDKModule->ProcessReady(ProcessParameters);
	
    UE_LOG(LogDS_GameMode, Log, TEXT("InitGameLift completed!"));
}

void ADS_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Auth Token: %s"), *OutServerParameters.m_authToken);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Host ID: %s"), *OutServerParameters.m_hostId);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Fleet ID: %s"), *OutServerParameters.m_fleetId)
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDS_GameMode, Log, TEXT("Websocket URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDS_GameMode, Log, TEXT("PID:c %s"), *OutServerParameters.m_processId);
}

void ADS_GameMode::ParseCommandLinePort(int32& OutPort)
{
	TArray<FString> CommandLineTokens;
	TArray<FString> CommandLineSwitches;
	FCommandLine::Parse(FCommandLine::Get(), CommandLineTokens, CommandLineSwitches);
	for (const FString& Switch : CommandLineSwitches)
	{
		FString Key;
		FString Value;
		if (Switch.Split("=", &Key, &Value))
		{
			if (Key.Equals(TEXT("port"), ESearchCase::IgnoreCase))
			{
				OutPort = FCString::Atoi(*Value);
				return;
			}
		}
	}
}
