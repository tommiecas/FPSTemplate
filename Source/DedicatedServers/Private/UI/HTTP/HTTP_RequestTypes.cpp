#include "UI/HTTP/HTTP_RequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

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
