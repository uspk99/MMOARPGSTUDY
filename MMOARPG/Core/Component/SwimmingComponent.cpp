// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimmingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "Camera/CameraComponent.h"

void USwimmingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USwimmingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CharacterMovementComponent.IsValid())
	{
		//姿态切换
		if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&&MMOARPGCharacterBase->GetActionState()!=ECharacterActionState::SWIMMING_STATE)
		{
			MMOARPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);
		}
		else if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE)
		{
			MMOARPGCharacterBase->ResetActionState(ECharacterActionState::SWIMMING_STATE);;
		}
		//游泳状态下逻辑
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE)
		{
			LockView(DeltaTime,*bDiving);
			ResetRotationRate(DeltaTime);
		}

	}
}

void USwimmingComponent::SwimForwardAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{
		if (InAxisValue>=0.f)
		{
			if (bDiving)
			{
				const FVector Direction = CameraComponent->GetForwardVector();
				MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);
			}
			else
			{//水面状态不能上下
				if (AController* Controller = MMOARPGCharacterBase->GetController())
				{
					// find out which way is forward
					const FRotator Rotation = Controller->GetControlRotation();
					const FRotator YawRotation(0, Rotation.Yaw, 0);
					// get forward vector
					const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
					MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);
				}
			}
		}
	}
}

void USwimmingComponent::ResetFastSwimming()
{
	if (CharacterMovementComponent.IsValid())
	{
		if (bFast)
		{
			bFast = false;
			CharacterMovementComponent->MaxSwimSpeed = 300.f;
		}
		else
		{
			bFast = true;
			CharacterMovementComponent->MaxSwimSpeed = 800.f;
		}
	}
}

void USwimmingComponent::GoUnderWater()
{
	if (bDiving)
	{
		bDiving = false;
		if (bFast)
		{
			CharacterMovementComponent->MaxSwimSpeed = 800.f;
		}
		else
		{
			CharacterMovementComponent->MaxSwimSpeed = 300.f;
		}
		//旋转阻力
		CharacterMovementComponent->RotationRate = FRotator(0.f, 0.f, 300.f);
	}
	else
	{
		bDiving = true;
		CharacterMovementComponent->MaxSwimSpeed = 800.f;

		CharacterMovementComponent->RotationRate = FRotator(0.f, 0.f, 540.f);
	}
}
