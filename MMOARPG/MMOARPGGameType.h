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

struct FResultBool
{
	FResultBool()
		:bSet(false),Time(0.f)
	{

	}
	void Tick(float DeltaTime)
	{
		//计时,并重置
		if (Time > 0.f)
		{
			Time -= DeltaTime;
			if (Time <= 0.f)
			{
				Fun.ExecuteIfBound();
				Time = 0.f;
				bSet = false;
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

