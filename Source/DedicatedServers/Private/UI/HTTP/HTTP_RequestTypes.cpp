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
	UE_LOG(LogDedicatedServers, Log, TEXT("Creation Time: %s"), *CreationTime);
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
	UE_LOG(LogDedicatedServers, Log, TEXT("Termination Time: %s"), *TerminationTime);
}

void FDS_PlayerSessionResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Player Session Response: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Creation Time: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("DNS Name: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("Fleet Arn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("Fleet ID: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Game Session ID: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IP Address: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Player Data: %s"), *PlayerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("Player ID: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Player Session ID: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("Termination Time: %s"), *TerminationTime);
}

void FDS_CodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Code Delivery Details: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Attribute Name: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("Delivery Medium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);
}

void FDS_SignUp_Response::Dump() const
{
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Sign Up Response: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("User Confirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogDedicatedServers, Log, TEXT("User Sub: %s"), *UserSub);
}


void FDS_NewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("New Device Metadata: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Device Group Key: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("Device Key: %s"), *DeviceKey);
}

void FDS_AuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Authentication Result: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("Access Token: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("Expires In: %d"), ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("Id Token: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Refresh Token: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("Token Type: %s"), *TokenType);
}

void FDS_InitiateAuth_Response::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Sign In Response: "));
	AuthenticationResult.Dump();

	UE_LOG(LogDedicatedServers, Log, TEXT("Available Challenges: "));
	for (const FString& Challenge : AvailableChallenges)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Challenge: %s"), *Challenge);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("Challenge Name: %s"), *ChallengeName);

	UE_LOG(LogDedicatedServers, Log, TEXT("Challenge Parameters: "));
	for (const auto& Parameter : ChallengeParameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("%s: %s"), *Parameter.Key, *Parameter.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
}
