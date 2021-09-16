// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGFlyAnimInstance.h"
#include "GameFramework/MovementComponent.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/FlyComponent.h"

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
		//不能直接GetVelocity，因为在绝对坐标里
		FVector SpeedVector = InCharacterBase->GetVelocity();
		FRotator CharacterRot = InCharacterBase->GetActorRotation();
		//转换为相对角色坐标
		SpeedVector = CharacterRot.UnrotateVector(SpeedVector);

		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			float MaxFSpeed = InCMC->MaxFlySpeed;
			//原本的范围线性映射到 -1到+1；
			FlySpeed.X = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFSpeed, MaxFSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.X);
			FlySpeed.Y = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFSpeed, MaxFSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Y);
			FlySpeed.Z = FMath::GetMappedRangeValueClamped(FVector2D(-MaxFSpeed, MaxFSpeed),
				FVector2D(-1.f, 1.f), SpeedVector.Z);
		}

		RotationRate =InCharacterBase->GetFlyComponent()->RotationRate;
		bFastFly = InCharacterBase->GetFlyComponent()->bFastFly;
	}
}
