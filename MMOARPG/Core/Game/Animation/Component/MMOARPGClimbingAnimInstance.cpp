// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGClimbingAnimInstance.h"
#include "GameFramework/MovementComponent.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../../../Component/ClimbingComponent.h"
#include "../../../../MMOARPGGameType.h"

void UMMOARPGClimbingAnimInstance::InitAnimInstance(ACharacter* InCharacter)
{
	Super::InitAnimInstance(InCharacter);
}

void UMMOARPGClimbingAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMMOARPGClimbingAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		ClimbingState = InCharacterBase->GetClimbingComponent()->ClimbingState;
		bJumpToClimbing = InCharacterBase->GetClimbingComponent()->bJumpToClimbing;
		bJump = *InCharacterBase->GetClimbingComponent()->bJump;
		//关联属性
		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			ResetAxisSpeed(InCMC->MaxCustomMovementSpeed);			
		}
		if (bJump)
		{
			InCharacterBase->GetClimbingComponent()->bJump = false;
			ClimbJump();
		}
		if (ClimbingState==EClimbingState::CLIMBING_TOTOP)
		{
			InCharacterBase->GetClimbingComponent()->ClearClimbingState();
			InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_CLIMB_UP_AT_TOP);
		}
		else if (ClimbingState == EClimbingState::CLIMBING_WALLCLIMBING)
		{
			InCharacterBase->GetClimbingComponent()->ClearClimbingState();
			if (InCharacterBase->GetClimbingComponent()->IsLowClimbing())
			{
				InCharacterBase->ClimbingMontageChanged(
					(EClimbingMontageState)FMath::RandRange(
						(int32)EClimbingMontageState::CLIMBING_ALS_N_MANTLE_1M_LH,
						(int32)EClimbingMontageState::CLIMBING_ALS_N_MANTLE_1M_RH));
			}
			else
			{
				InCharacterBase->ClimbingMontageChanged(
					EClimbingMontageState::CLIMBING_ALS_N_MANTLE_2M);
			}
		}
		else if (ClimbingState == EClimbingState::CLIMBING_TRUN)
		{
			EClimbTurnState TurnState = InCharacterBase->GetClimbingComponent()->GetTurnState();
			switch (TurnState)
			{
			case EClimbTurnState::TURN_NONE:
				break;
			case EClimbTurnState::OUTSIDE_RIGHT:
				InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_OUTSIDE_R_RM);
				break;
			case EClimbTurnState::OUTSIDE_LEFT:
				InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_OUTSIDE_L_RM);
				break;
			case EClimbTurnState::INSIDE_RIGHT:
				InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_INSIDE_R_RM);
				break;
			case EClimbTurnState::INSIDE_LEFT:
				InCharacterBase->ClimbingMontageChanged(EClimbingMontageState::CLIMBING_TURN_CORNER_INSIDE_L_RM);
				break;
			default:
				break;
			}
			InCharacterBase->GetClimbingComponent()->ResetClimbingState();
		}
	}
}

void UMMOARPGClimbingAnimInstance::ClimbJump()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		EClimbingMontageState JumpState = CalculationClimbingJumpState();
		if (JumpState!=EClimbingMontageState::CLIMBING_DASH_MAX)
		{
			InCharacterBase->ClimbingMontageChanged(JumpState);
		}
	}
}

EClimbingMontageState UMMOARPGClimbingAnimInstance::CalculationClimbingJumpState()
{
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(TryGetPawnOwner()))
	{
		if (UCharacterMovementComponent* InCMC = Cast<UCharacterMovementComponent>(InCharacterBase->GetMovementComponent()))
		{
			
			//FVector2D Axis(InCMC->Velocity.Y, InCMC->Velocity.Z);
			FVector2D Axis(InCMC->GetLastInputVector().Y, InCMC->GetLastInputVector().Z);
			
			//区分
			FVector2D XAxis(1.f, 0.f);
			Axis.Normalize();
			float CosValueX = FVector2D::DotProduct(Axis, XAxis);
			float XAxisCosAngle = (180.f) / PI * FMath::Acos(CosValueX);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.04f, FColor::Red, FString::SanitizeFloat(XAxisCosAngle));
			}

			//区分
			FVector2D YAxis(0.f, 1.f);
			Axis.Normalize();
			float CosValueY = FVector2D::DotProduct(Axis, YAxis);
			float YAxisCosAngle = (180.f) / PI * FMath::Acos(CosValueY);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.04f, FColor::Red, FString::SanitizeFloat(YAxisCosAngle));
			}
			//判断数值是否在范围内
			bool bUpAxis = FMath::IsWithinInclusive(YAxisCosAngle, 0.f, 90.f);

			if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_UR_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && !bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_DL_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 22.5f, 67.5f) && !bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_DR_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 112.5f, 157.5f) && bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_UL_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f)&&bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_U_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 67.5f, 112.5f) &&!bUpAxis)
			{
				return EClimbingMontageState::CLIMBING_DASH_D_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 157.5f, 180.f))
			{
				return EClimbingMontageState::CLIMBING_DASH_L_RM;
			}
			else if (FMath::IsWithinInclusive(XAxisCosAngle, 0.f, 22.5f))
			{
				return EClimbingMontageState::CLIMBING_DASH_R_RM;
			}
		}
	}
	return EClimbingMontageState::CLIMBING_DASH_MAX;
}
