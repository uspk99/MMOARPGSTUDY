// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGSwimmingAnimInstance.h"
#include "GameFramework/MovementComponent.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/SwimmingComponent.h"

void UMMOARPGSwimmingAnimInstance::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);
}

void UMMOARPGSwimmingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMOARPGSwimmingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		//¹ØÁªÊôÐÔ
		bFast = *InCharacterBase->GetSwimmingComponent()->bFast;
		bDiving = *InCharacterBase->GetSwimmingComponent()->bDiving;
		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			ResetAxisSpeed(InCMC->MaxSwimSpeed);
		}
	}
}
