#include "MMOARPGMovementComponent.h"

UMMOARPGMovementComponent::UMMOARPGMovementComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, MOARPGPendingLaunchVelocity(FVector::ZeroVector)
{

}

void UMMOARPGMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementMode != EMovementMode::MOVE_Flying)
	{
		AdjustmentPendingLaunchVelocity(DeltaTime);
	}
}

void UMMOARPGMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (deltaTime < UCharacterMovementComponent::MIN_TICK_TIME)
	{
		return;
	}

	if (!HasAnimRootMotion())
	{
		Velocity = MOARPGPendingLaunchVelocity + GetLastInputVector() * MaxCustomMovementSpeed;
	}
	else
	{
		Velocity = MOARPGPendingLaunchVelocity + ConstrainAnimRootMotionVelocity(AnimRootMotionVelocity, Velocity);
	}

	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
}

void UMMOARPGMovementComponent::Launch(FVector const& LaunchVel)
{
	Super::Launch(LaunchVel);

	//if ((MovementMode != MOVE_None) && IsActive() && HasValidData())
	//{
	//	MOARPGPendingLaunchVelocity = LaunchVel;
	//}
}

void UMMOARPGMovementComponent::AdjustmentPendingLaunchVelocity(float DeltaTime)
{
	auto AxisCheck = [](float& InValue, float DeltaTime)
	{
		if (FMath::IsNearlyEqual(InValue, 0.f, 1.f))
		{
			InValue = 0.f;
		}
		else if (InValue > 0.f)
		{
			InValue -= InValue * DeltaTime;
		}
		else if (InValue < 0.f)
		{
			InValue += -InValue * DeltaTime;
		}
	};

	AxisCheck(MOARPGPendingLaunchVelocity.X, DeltaTime);
	AxisCheck(MOARPGPendingLaunchVelocity.Y, DeltaTime);
	AxisCheck(MOARPGPendingLaunchVelocity.Z, DeltaTime);
}
