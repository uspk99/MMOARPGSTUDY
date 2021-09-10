// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGAnimInstanceBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"

UMMOARPGAnimInstanceBase::UMMOARPGAnimInstanceBase()
	:Speed(0.f),bDeath(false),bInAir(false),bFight(false)
{

}

void UMMOARPGAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMOARPGAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		//更新速度
		Speed = InCharacterBase->GetVelocity().Size();
		bInAir = InCharacterBase->GetMovementComponent()->IsFalling();
		bFight = InCharacterBase->IsFight();
	}

}
