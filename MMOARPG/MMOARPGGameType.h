// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameType.generated.h"

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	NORMAL_STATE	UMETA(DisplayName = "Normal"),
	FIGHT_STATE		UMETA(DisplayName = "Fight"),
	FLIGHT_STATE	UMETA(DisplayName = "Flight"),
	CLIMB_STATE		UMETA(DisplayName = "Climb"),
	SWIMMING_STATE	UMETA(DisplayName = "Swimming"),
};