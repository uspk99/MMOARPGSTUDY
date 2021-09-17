// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceComponentBase.h"
#include "GameFramework/MovementComponent.h"
#include "../../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../../Component/FlyComponent.h"
#include "../../../../Component/SwimmingComponent.h"

UAnimInstanceComponentBase::UAnimInstanceComponentBase()
	:Super()

{

}

void UAnimInstanceComponentBase::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);
}

void UAnimInstanceComponentBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAnimInstanceComponentBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UAnimInstanceComponentBase::ResetAxisSpeed(float NewSpeed)
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		//不能直接GetVelocity，因为在绝对坐标里
		FVector SpeedVector = InCharacterBase->GetVelocity();
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		//转换为相对角色坐标
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);

		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			//float MaxFSpeed = InCMC->MaxFlySpeed;			
			//原本的范围线性映射到 -1到+1；
			AxisSpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.X);
			AxisSpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Y);
			AxisSpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Z);
		}
		//关联属性
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;
	}
}
