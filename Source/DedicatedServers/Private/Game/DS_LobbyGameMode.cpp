// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DS_LobbyGameMode.h"

#include "Game/DS_GameInstanceSubsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "DedicatedServersTypes/DS_Types.h"
#include "Game/DS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyState.h"
#include "Players/DS_LocalPlayerSubsystem.h"
#include "Players/DS_PlayerController.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers;
	MinPlayers = 1;
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown;
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CheckAndStartLobbyCountdown();
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	CheckAndStartLobbyCountdown();

	if (LobbyStatus != ELobbyStatus::SeamlessTraveling)
	{
		AddPlayerInfoToLobbyState(NewController);
	}
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckAndStopLobbyCountdown();
	RemovePlayerSession(Exiting);
	if (LobbyStatus != ELobbyStatus::SeamlessTraveling)
	{
		RemovePlayerInfoFromLobbyState(Exiting);
	}
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("Username"));
	
	TryAcceptPlayerSession(PlayerSessionId, UserName, ErrorMessage);
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString InitializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString PlayerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(NewPlayerController); IsValid(DS_PlayerController))
	{
		DS_PlayerController->PlayerSessionId = PlayerSessionId;
		DS_PlayerController->Username = UserName;
	}
	if (LobbyStatus != ELobbyStatus::SeamlessTraveling)
	{
		AddPlayerInfoToLobbyState(NewPlayerController);
	}
	return InitializedString;
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& UserName, FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || UserName.IsEmpty())
	{
		OutErrorMessage = TEXT("Player Session ID and/or User Name invalid.");
		return;
	}
	
#if WITH_GAMELIFT

	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest DescribePlayerSessionsRequest;

	DescribePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId));

	const auto& DescribePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(DescribePlayerSessionsRequest);
	if (!DescribePlayerSessionsOutcome.IsSuccess())
	{
		OutErrorMessage = TEXT("Describe Player Sessions failed.");
		return;
	}
	
	const auto& DescribePlayerSessionsResult = DescribePlayerSessionsOutcome.GetResult();
	int32 Count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* PlayerSessions = DescribePlayerSessionsResult.GetPlayerSessions(Count);
	if (PlayerSessions == nullptr || Count == 0)
	{
		OutErrorMessage = TEXT("Get Player Sessions failed.");
		return;
	}

	for (int32 i = 0; i < Count; i++)
	{
		const Aws::GameLift::Server::Model::PlayerSession& PlayerSession = PlayerSessions[i];
		if (!UserName.Equals(PlayerSession.GetPlayerId())) continue;
		if (PlayerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED)
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin."), *UserName);
			return;
		}

		const auto& AcceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId));
		OutErrorMessage = AcceptPlayerSessionOutcome.IsSuccess()
			? TEXT("")
			: FString::Printf(TEXT("Failed to accept player session for %s"), *UserName);	}
#endif
}

void ADS_LobbyGameMode::AddPlayerInfoToLobbyState(AController* Player) const
{
	ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(Player);
	ADS_GameState* DS_GameState = GetGameState<ADS_GameState>();
	if (IsValid(DS_GameState) && IsValid(DS_GameState->LobbyState) && IsValid(DS_PlayerController))
	{
		FLobbyPlayerInfo PlayerInfo(DS_PlayerController->Username);
		DS_GameState->LobbyState->AddPlayerInfo(PlayerInfo);
	}
}

void ADS_LobbyGameMode::RemovePlayerInfoFromLobbyState(AController* Player) const
{
	ADS_PlayerController* DS_PlayerController = Cast<ADS_PlayerController>(Player);
	ADS_GameState* DS_GameState = GetGameState<ADS_GameState>();
	if (IsValid(DS_GameState) && IsValid(DS_GameState->LobbyState) && IsValid(DS_PlayerController))
	{
		DS_GameState->LobbyState->RemovePlayerInfo(DS_PlayerController->Username);
	}
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer);
	}
}

void ADS_LobbyGameMode::CheckAndStopLobbyCountdown()
{
	if (GetNumPlayers() - 1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer);
	}
}
void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		StopCountdownTimer(LobbyCountdownTimer);
		LobbyStatus = ELobbyStatus::SeamlessTraveling;
		TrySeamlessTravel(DestinationMap);
	}
}

void ADS_LobbyGameMode::InitGameLift()
{
	if (UGameInstance* GameInstance = GetGameInstance(); IsValid(GameInstance))
	{
		if (DS_GameInstanceSubsystem = GameInstance->GetSubsystem<UDS_GameInstanceSubsystem>(); IsValid(DS_GameInstanceSubsystem))
		{
			FServerParameters ServerParameters;
			SetServerParameters(ServerParameters);
			DS_GameInstanceSubsystem->InitializeGameLift(ServerParameters);
		}
	}
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Auth Token: %s"), *OutServerParameters.m_authToken);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Host ID: %s"), *OutServerParameters.m_hostId);
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Fleet ID: %s"), *OutServerParameters.m_fleetId)
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Websocket URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	OutServerParameters.m_processId = FString::Printf(TEXT("%d"), GetCurrentProcessId());
	UE_LOG(LogDedicatedServers, Log, TEXT("PID:c %s"), *OutServerParameters.m_processId);
}
