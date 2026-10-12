#pragma once

#include "DS_Types.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	Started UMETA(DisplayName = "Started"),
	Paused UMETA(DisplayName = "Paused"),
	Stopped UMETA(DisplayName = "Stopped"),

	DefaultMax UMETA(DisplayName = "Max")
};

UENUM(BlueprintType)
enum class ECountdownTimerType : uint8
{
	LobbyCountdown UMETA(DisplayName = "Lobby Countdown"),
	PreMatch UMETA(DisplayName = "Pre Match"),
	Match UMETA(DisplayName = "Match"),
	PostMatch UMETA(DisplayName = "Post Match"),
	None UMETA(DisplayName = "None"),

	DefaultMax UMETA(DisplayName = "MAX")
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers UMETA(DisplayName = "Waiting for Players"),
	PreMatch UMETA(DisplayName = "Pre Match"),
	Match UMETA(DisplayName = "Match"),
	PostMatch UMETA(DisplayName = "Post Match"),
	SeamlessTraveling UMETA(DisplayName = "Seamless Traveling"),

	DefaultMax UMETA(DisplayName = "MAX")
};

UENUM()
enum class ELobbyStatus : uint8
{
	WaitingForPlayers UMETA(DisplayName = "Waiting for Players"),
	CountdownToSeamlessTravel UMETA(DisplayName = "Countdown to Seamless Travel"),
	SeamlessTraveling UMETA(DisplayName = "Seamless Traveling")
};

USTRUCT(BlueprintType)
struct FCountdownTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerState State = ECountdownTimerState::NotStarted;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ECountdownTimerType Type = ECountdownTimerType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownUpdateInterval = 1.f;

	FTimerHandle TimerFinishedHandle{};
	FTimerHandle TimerUpdateHandle{};
	FTimerDelegate TimerFinishedDelegate{};
	FTimerDelegate TimerUpdateDelegate{};

	
};

inline bool operator==(const FCountdownTimerHandle& LHS, const FCountdownTimerHandle& RHS)
{
	return LHS.Type == RHS.Type;
}