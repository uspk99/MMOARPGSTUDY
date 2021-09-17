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
		//����ֱ��GetVelocity����Ϊ�ھ���������
		FVector SpeedVector = InCharacterBase->GetVelocity();
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		//ת��Ϊ��Խ�ɫ����
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);

		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			//float MaxFSpeed = InCMC->MaxFlySpeed;			
			//ԭ���ķ�Χ����ӳ�䵽 -1��+1��
			AxisSpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.X);
			AxisSpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Y);
			AxisSpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-NewSpeed, NewSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Z);
		}
		//��������
		RotationRate = InCharacterBase->GetFlyComponent()->RotationRate;
	}
}
