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
		//×ËÌ¬ÇÐ»»
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
		//ÓÎÓ¾×´Ì¬ÏÂÂß¼­
		else if (CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Swimming
			&& MMOARPGCharacterBase->GetActionState() == ECharacterActionState::SWIMMING_STATE)
		{
		}

	}
}
