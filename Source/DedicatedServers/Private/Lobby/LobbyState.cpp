// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"


ALobbyState::ALobbyState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALobbyState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyState, PlayerInfoArray);
}

void ALobbyState::AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);
}

void ALobbyState::RemovePlayerInfo(const FString& PlayerName)
{
	PlayerInfoArray.RemovePlayer(PlayerName);
}

TArray<FLobbyPlayerInfo> ALobbyState::GetPlayers() const
{
	return PlayerInfoArray.Players;
}

void ALobbyState::OnRep_LobbyPlayerInfo()
{
	FLobbyPlayerInfoDelta Delta = CalculatePlayerInfoDelta(LastLobbyPlayerInfoArray.Players, PlayerInfoArray.Players);
	for (const auto& PlayerInfo : Delta.AddedPlayers)
	{
		OnPlayerInfoAdded.Broadcast(PlayerInfo);
	}
	for (const auto& PlayerInfo : Delta.RemovedPlayers)
	{
		OnPlayerInfoRemoved.Broadcast(PlayerInfo);
	}
	LastLobbyPlayerInfoArray = PlayerInfoArray;
}

FLobbyPlayerInfoDelta ALobbyState::CalculatePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& OldPlayerInfoArray,
	const TArray<FLobbyPlayerInfo>& NewPlayerInfoArray)
{
	FLobbyPlayerInfoDelta Delta;
	TMap<FString, const FLobbyPlayerInfo*> OldPlayerInfoMap;
	TMap<FString, const FLobbyPlayerInfo*> NewPlayerInfoMap;

	for (const auto& PlayerInfo : OldPlayerInfoArray)
	{
		OldPlayerInfoMap.Add(PlayerInfo.UserName, &PlayerInfo);
	}
	for (const auto& PlayerInfo : NewPlayerInfoArray)
	{
		NewPlayerInfoMap.Add(PlayerInfo.UserName, &PlayerInfo);
	}
	for (const auto& OldPlayerInfo : OldPlayerInfoArray)
	{
		if (!NewPlayerInfoMap.Contains(OldPlayerInfo.UserName))
		{
			Delta.RemovedPlayers.Add(OldPlayerInfo);
		}
	}
	for (const auto& NewPlayerInfo : NewPlayerInfoArray)
	{
		if (!OldPlayerInfoMap.Contains(NewPlayerInfo.UserName))
		{
			Delta.AddedPlayers.Add(NewPlayerInfo);
		}
	}

	return Delta;
}




