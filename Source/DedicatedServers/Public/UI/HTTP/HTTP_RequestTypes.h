#pragma once

#include "HTTP_RequestTypes.generated.h"

namespace HTTP_StatusMessages
{
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong;
}

USTRUCT()
struct FDS_MetaData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 httpStatusCode{};

	UPROPERTY()
	FString requestId{};
	
	UPROPERTY()
	int32 attempts{};

	UPROPERTY()
	double totalRetryDelay{};

	void Dump() const;
};

USTRUCT()
struct FDS_ListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken{};

	void Dump() const;
};

USTRUCT()
struct FDS_GameSessionResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString CreatorId{};

	UPROPERTY()
	int32 CurrentPlayerSessionCount{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	TMap<FString, FString> GameProperties{};

	UPROPERTY()
	FString GameSessionData{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString Location{};

	UPROPERTY()
	FString MatchmakerData{};

	UPROPERTY()
	int32 MaximumPlayerSessionCount{};

	UPROPERTY()
	FString Name{};

	UPROPERTY()
	FString PlayerSessionCreationPolicy{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString StatusReason{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;	
};

USTRUCT()
struct FDS_PlayerSessionResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString PlayerData{};

	UPROPERTY()
	FString PlayerId{};

	UPROPERTY()
	FString PlayerSessionId{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;
};


USTRUCT()
struct FDS_CodeDeliveryDetails
{
	GENERATED_BODY()

	UPROPERTY()
	FString AttributeName{};

	UPROPERTY()
	FString DeliveryMedium{};

	UPROPERTY()
	FString Destination{};

	void Dump() const;
};

USTRUCT()
struct FDS_SignUp_Response
{
	GENERATED_BODY()

	UPROPERTY()
	FDS_CodeDeliveryDetails CodeDeliveryDetails{};

	UPROPERTY()
	FString Session{};

	UPROPERTY()
	bool UserConfirmed{};

	UPROPERTY()
	FString UserSub{};

	void Dump() const;
};

USTRUCT()
struct FDS_NewDeviceMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString DeviceGroupKey{};

	UPROPERTY()
	FString DeviceKey{};

	void Dump() const;
};

USTRUCT()
struct FDS_AuthenticationResult
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccessToken{};

	UPROPERTY()
	int32 ExpiresIn{};

	UPROPERTY()
	FString IdToken{};

	UPROPERTY()
	FDS_NewDeviceMetadata NewDeviceMetadata{};

	UPROPERTY()
	FString RefreshToken{};

	UPROPERTY()
	FString TokenType{};

	void Dump() const;
};

USTRUCT()
struct FDS_InitiateAuth_Response
{
	GENERATED_BODY()

	UPROPERTY()
	FDS_AuthenticationResult AuthenticationResult{};

	UPROPERTY()
	TArray<FString> AvailableChallenges{};

	UPROPERTY()
	FString ChallengeName{};

	UPROPERTY()
	TMap<FString, FString> ChallengeParameters{};

	UPROPERTY()
	FString Session{};

	UPROPERTY()
	FString email{};

	void Dump() const;
};

USTRUCT()
struct FDS_MatchStats
{
	GENERATED_BODY()

	UPROPERTY()
	int32 scoredEliminations{};

	UPROPERTY()
	int32 defeats{};

	UPROPERTY()
	int32 hits{};

	UPROPERTY()
	int32 misses{};

	UPROPERTY()
	int32 headShotEliminations{};

	UPROPERTY()
	int32 gotFirstBlood{};

	UPROPERTY()
	int32 matchWins{};

	UPROPERTY()
	int32 matchLosses{};

	UPROPERTY()
	int32 longestStreak{};

	UPROPERTY()
	int32 revengeEliminations{};

	UPROPERTY()
	int32 dethroneEliminations{};

	UPROPERTY()
	int32 showStopperEliminations{};
};

USTRUCT()
struct FDS_RecordMatchStatsInput
{
	GENERATED_BODY()
	
	UPROPERTY()
	FDS_MatchStats matchStats{};

	UPROPERTY()
	FString username{};
};

USTRUCT()
struct FDSRetrieveMatchStatsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	int32 longestStreak{};

	UPROPERTY()
	int32 dethroneEliminations{};

	UPROPERTY()
	int32 gotFirstBlood{};

	UPROPERTY()
	int32 defeats{};

	UPROPERTY()
	int32 scoredEliminations{};

	UPROPERTY()
	FString email{};

	UPROPERTY()
	int32 misses{};

	UPROPERTY()
	int32 revengeEliminations{};

	UPROPERTY()
	int32 matchLosses{};

	UPROPERTY()
	int32 showstopperEliminations{};

	UPROPERTY()
	FString databaseid{};

	UPROPERTY()
	int32 headShotEliminations{};

	UPROPERTY()
	FString username{};

	UPROPERTY()
	int32 hits{};

	UPROPERTY()
	int32 matchWins{};

	void Dump() const;
};