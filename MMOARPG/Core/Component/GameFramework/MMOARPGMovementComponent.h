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
	/**
	 * Default UObject constructor.
	 */
	UMMOARPGMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PhysCustom(float deltaTime, int32 Iterations);

	/** Queue a pending launch with velocity LaunchVel. */
	virtual void Launch(FVector const& LaunchVel);

protected:
	void AdjustmentPendingLaunchVelocity(float DeltaTime);

private:
	FVector MOARPGPendingLaunchVelocity;
};