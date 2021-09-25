// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MMOARPGMovementComponent.generated.h"

UCLASS()
class UMMOARPGMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	virtual void PhysCustom(float deltaTime, int32 Iterations);
};