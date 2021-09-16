// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGAnimInstanceBase.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SimpleAdvancedAnimationBPLibrary.h"

UMMOARPGAnimInstanceBase::UMMOARPGAnimInstanceBase()
	:Speed(0.f),
	bDeath(false),
	bInAir(false),	
	bFootIK(false),
	FootIKID(INDEX_NONE),
	ActionState(ECharacterActionState::NORMAL_STATE),
	LeftBoneName(TEXT("foot_l")),
	RightBoneName(TEXT("foot_r"))
	//bFight(false),
{

}

void UMMOARPGAnimInstanceBase::InitAnimInstance(ACharacter* InCharacter)
{
	if (bFootIK)
	{
		BoneNames.Add(LeftBoneName);
		BoneNames.Add(RightBoneName);
		FootIKID = USimpleAdvancedAnimationBPLibrary::CreateFootIK(InCharacter, BoneNames);
	}
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
		//bFight = InCharacterBase->GetActionState();
		ActionState = InCharacterBase->GetActionState();
	}

	if (bFootIK&& FootIKID!=INDEX_NONE)
	{
		TArray<float> OffsetArray;		
		//考虑双腿
		float LOffset = GetFootIKOffset(LeftBoneName);
		float ROffset = GetFootIKOffset(RightBoneName);

		OffsetArray.Add(LOffset);
		OffsetArray.Add(ROffset);

		ButtZOffset = USimpleAdvancedAnimationBPLibrary::GetButtZOffset(OffsetArray);


		LeftOffset = -(ButtZOffset - LOffset);
		RightOffset = ButtZOffset - ROffset;
	}

}

float UMMOARPGAnimInstanceBase::GetFootIKOffset(const FName& InBoneName)
{
	if (FootIKID!=INDEX_NONE)
	{
		return USimpleAdvancedAnimationBPLibrary::FindOffset(FootIKID, InBoneName);
	}
	return 0.0f;
}