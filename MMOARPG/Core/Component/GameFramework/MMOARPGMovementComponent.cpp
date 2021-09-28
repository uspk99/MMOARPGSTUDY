#include "MMOARPGMovementComponent.h"

void UMMOARPGMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (deltaTime < UCharacterMovementComponent::MIN_TICK_TIME)
	{
		return;
	}
	

	if (!HasAnimRootMotion())
	{
		Velocity = PendingLaunchVelocity + GetLastInputVector() * MaxCustomMovementSpeed;
	}
	else
	{
		Velocity = PendingLaunchVelocity + ConstrainAnimRootMotionVelocity(AnimRootMotionVelocity,Velocity);
	}

	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
}