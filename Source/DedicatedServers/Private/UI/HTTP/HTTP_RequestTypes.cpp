#include "UI/HTTP/HTTP_RequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTP_StatusMessages
{
	const FString SomethingWentWrong{TEXT("Something went wrong!")};
}

void FDS_MetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MetaData:"));

	UE_LOG(LogDedicatedServers, Log, TEXT("HTTP Status Code: %d"), httpStatusCode);

	UE_LOG(LogDedicatedServers, Log, TEXT("Request ID: %s"), *requestId);

	UE_LOG(LogDedicatedServers, Log, TEXT("Attempts: %d"), attempts);

	UE_LOG(LogDedicatedServers, Log, TEXT("Total Retry Delay: %f"), totalRetryDelay);
}

void FDS_ListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("List Fleets Response: "));

	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Fleet ID: %s"), *FleetId);
	}
	
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Next Token: %s"), *NextToken);
	}
}

void FDS_GameSessionResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Game Session Response: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Creation Time: %f"), CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("Creator ID: %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Current Player Session Count: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("DNS Name: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("Fleet Arn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("Fleet ID: %s"), *FleetId);

	UE_LOG(LogDedicatedServers, Log, TEXT("Game Properties: "));
	for (const auto& GameProperty : GameProperties)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("%s: %s"), *GameProperty.Key, *GameProperty.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("Game Session Data: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("Game Session ID: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IP Address: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("Matchmaker Data: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("Maximum Player Session Count: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("Player Session Creation Policy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("Termination Time: %f"), TerminationTime);
}
