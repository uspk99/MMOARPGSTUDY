// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGFlyAnimInstance.h"
#include "GameFramework/MovementComponent.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/FlyComponent.h"

UMMOARPGFlyAnimInstance::UMMOARPGFlyAnimInstance()
	:Super()
	, DodgeFly(EDodgeFly::DODGE_NONE)
{

}

void UMMOARPGFlyAnimInstance::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);
}

void UMMOARPGFlyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMOARPGFlyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		//¹ØÁªÊôÐÔ

		DodgeFly = InCharacterBase->GetFlyComponent()->DodgeFly;
		bLand = *InCharacterBase->GetFlyComponent()->bLand;
		bFast = *InCharacterBase->GetFlyComponent()->bFast;
		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			ResetAxisSpeed(InCMC->MaxFlySpeed);
		}
	}

}
