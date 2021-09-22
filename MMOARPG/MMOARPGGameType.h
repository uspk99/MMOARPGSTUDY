// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
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


UENUM(BlueprintType)
enum class EDodgeFly : uint8
{
	DODGE_NONE		UMETA(DisplayName = "None"),
	DODGE_LEFT		UMETA(DisplayName = "Left"),
	DODGE_RIGHT		UMETA(DisplayName = "Right"),
};

UENUM(BlueprintType)
enum class EClimbingState : uint8
{
	CLIMBING_NONE					UMETA(DisplayName = "None"),
	CLIMBING_CLIMBOVEROBSTACLES		UMETA(DisplayName = "Climb Over Obstacles"),
	CLIMBING_WALLCLIMBING			UMETA(DisplayName = "Wall Climbing"),
	CLIMBING_CLIMBING				UMETA(DisplayName = "Climbing"),
	CLIMBING_TOGROUND				UMETA(DisplayName = "To Ground"),
	CLIMBING_TOTOP					UMETA(DisplayName = "To Top"),
	CLIMBING_DROP					UMETA(DisplayName = "Drop"),
	CLIMBING_TRUN					UMETA(DisplayName = "Trun"),
};
UENUM(BlueprintType)
enum class EClimbingMontageState : uint8
{
	CLIMBING_DASH_D_RM				UMETA(DisplayName = "Climbing Dash Down	"),
	CLIMBING_DASH_DR_RM				UMETA(DisplayName = "Climbing Dash Down	Right"),
	CLIMBING_DASH_DL_RM				UMETA(DisplayName = "Climbing Dash Down Left"),
	CLIMBING_DASH_L_RM				UMETA(DisplayName = "Climbing Dash Left"),
	CLIMBING_DASH_R_RM				UMETA(DisplayName = "Climbing Dash Right"),
	CLIMBING_DASH_U_RM				UMETA(DisplayName = "Climbing Dash Up"),
	CLIMBING_DASH_UL_RM				UMETA(DisplayName = "Climbing Dash Down	Left"),
	CLIMBING_DASH_UR_RM				UMETA(DisplayName = "Climbing Dash Down	Right"),
	CLIMBING_CLIMB_UP_AT_TOP		UMETA(DisplayName = "Climbing Climb Up At Top"),

	CLIMBING_ALS_N_MANTLE_1M_LH		UMETA(DisplayName = "Climbing Left Turn"),
	CLIMBING_ALS_N_MANTLE_1M_RH		UMETA(DisplayName = "Climbing Right Turn"),

	CLIMBING_ALS_N_MANTLE_2M		UMETA(DisplayName = "Climbing Turn"),

	CLIMBING_DASH_DROP_RM				UMETA(DisplayName = "Climbing Drop"),

	CLIMBING_TURN_CORNER_OUTSIDE_R_RM			UMETA(DisplayName = "Trun Corner Outside Right"),
	CLIMBING_TURN_CORNER_OUTSIDE_L_RM			UMETA(DisplayName = "Turn Corner Outside Left"),
	CLIMBING_TURN_CORNER_INSIDE_R_RM			UMETA(DisplayName = "Turn Corner Inside Right"),
	CLIMBING_TURN_CORNER_INSIDE_L_RM			UMETA(DisplayName = "Turn Corner Inside Left"),

	CLIMBING_DASH_MAX				UMETA(DisplayName = "_MAX"),
};

UENUM(BlueprintType)
enum class EClimbTurnState : uint8
{
	TURN_NONE,
	OUTSIDE_RIGHT,
	OUTSIDE_LEFT,
	INSIDE_RIGHT,
	INSIDE_LEFT,
};

struct FResultBool
{
	FResultBool()
		:bSet(false),Time(0.f)
	{

	}
	void Tick(float DeltaTime,bool bReset)
	{
		//计时,并重置
		if (Time > 0.f)
		{
			Time -= DeltaTime;
			if (Time <= 0.f)
			{
				Fun.ExecuteIfBound();
				Time = 0.f;
				if (bReset)
				{
					bSet = false;
				}
			}
		}
	}

	FResultBool &operator=(bool NewSet)
	{
		bSet = NewSet;
		return *this;
	}

	FResultBool& operator=(float InTime)
	{
		Time = InTime;
		return *this;
	}

	bool operator*()
	{
		return bSet;
	}
	//支持直接在if中判断
	//显性调用
	explicit operator bool()
	{
		return bSet;
	}

	bool bSet;
	float Time;
	FSimpleDelegate Fun;
};

