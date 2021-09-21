// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/MovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ThreadManage.h"

const float MaxDistance = 9999.f;

UClimbingComponent::UClimbingComponent()
	:Super()
	,ClimbingState(EClimbingState::CLIMBING_NONE)
	, bJumpToClimbing(false)
	,ClimbingHeight(0.f)
{
}

void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{
		TraceClimbingState(DeltaTime);
	}

	bJump.Tick(DeltaTime,true);
	bWallClimbing.Tick(DeltaTime, true);
}

void UClimbingComponent::ResetJump()
{
	bJump = true;
	bJump = 1.6f;
}

void UClimbingComponent::ClimbingForwardAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{
		if (AController* Controller = MMOARPGCharacterBase->GetController())
		{
			//��������
			const FRotator Rotation = MMOARPGCharacterBase->GetActorRotation();
			const FRotator YawRotation(Rotation.Pitch, Rotation.Yaw, 0);
			// get up vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Z);
			MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);
		}
	}
}

void UClimbingComponent::ClimbingRightAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{
		if (AController* Controller = MMOARPGCharacterBase->GetController())
		{
			// find out which way is forward
			//�õ�ACtor�������ת
			const FRotator Rotation = MMOARPGCharacterBase->GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);
		}
	}
}

void UClimbingComponent::PhysClimbing(float deltaTime, int32 Iterations)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{

		if (deltaTime<UCharacterMovementComponent::MIN_TICK_TIME)
		{//С����Сʱ�䣬���ƶ���Ҫ
			return;
		}
		if (!CharacterMovementComponent->HasAnimRootMotion())
		{
			//����Velocity last��������ٶ�
			CharacterMovementComponent->Velocity = CharacterMovementComponent->GetLastInputVector()
				* CharacterMovementComponent->MaxCustomMovementSpeed;
		}
		else
		{
			//������rootMotion
			CharacterMovementComponent->Velocity =
				CharacterMovementComponent->ConstrainAnimRootMotionVelocity(
					CharacterMovementComponent->AnimRootMotionVelocity,
					CharacterMovementComponent->Velocity);
		}

		//ÿ֡����
		const FVector Adjusted = CharacterMovementComponent->Velocity * deltaTime;

		FHitResult Hit(1.f);
		//GetComponentQuat������ת true��ɨ�� ��ײ���
		CharacterMovementComponent->SafeMoveUpdatedComponent
		(Adjusted,CharacterMovementComponent->UpdatedComponent->GetComponentQuat(),true,Hit);
		
	}


}

void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	FVector ForwardDireaction = MMOARPGCharacterBase->GetActorForwardVector();
	FVector UpDireaction = MMOARPGCharacterBase->GetActorUpVector();
	FVector LocaLocation = MMOARPGCharacterBase->GetActorLocation();

	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();
	
	FHitResult HitChestResult;
	TArray<AActor*> Ignores1;
	float ChestDistance = MaxDistance;
	{
		//�ؿ� 
		FVector StartTraceChestLocation = LocaLocation;
		StartTraceChestLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight() / 8.f;

		FVector EndTraceChestLocation = StartTraceChestLocation + ForwardDireaction * 100.f;
		
		if (UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartTraceChestLocation,
			EndTraceChestLocation,
			ETraceTypeQuery::TraceTypeQuery1,//��ѯͨ��
			false,//false:����ײ
			Ignores1,
			EDrawDebugTrace::Type::ForOneFrame,//ForOneFrameһ֡���� None������
			HitChestResult,
			true))//��������
		{
			if (HitChestResult.bBlockingHit)
			{
				ChestDistance = FVector::Distance(StartTraceChestLocation, HitChestResult.Location);

			}
		}

	}
	FHitResult HitHeadResult;
	TArray<AActor*> Ignores2;
	float HeadDistance = MaxDistance;
	{
		//ͷ��
		FVector StartTraceHeadLocation = LocaLocation;
		StartTraceHeadLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight()+1.f;

		FVector EndTraceHeadLocation = StartTraceHeadLocation + ForwardDireaction * 100.f;		
		if (UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartTraceHeadLocation,
			EndTraceHeadLocation,
			ETraceTypeQuery::TraceTypeQuery1,//��ѯͨ��
			false,//false:����ײ
			Ignores2,
			EDrawDebugTrace::Type::ForOneFrame,//ForOneFrameһ֡���� None������
			HitHeadResult,
			true))//��������
		{
			if (HitHeadResult.bBlockingHit)
			{
				HeadDistance = FVector::Distance(StartTraceHeadLocation, HitHeadResult.Location);
			}
		}
	}

	FHitResult HitGroundResult;
	TArray<AActor*> Ignores3;
	float GroundDistance = MaxDistance;
	{
		FVector StartTraceLocation = LocaLocation;
		StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();

		FVector EndTraceLocation = StartTraceLocation + (-UpDireaction) * 100.f;
		if (UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			StartTraceLocation,
			EndTraceLocation,
			ETraceTypeQuery::TraceTypeQuery1,//��ѯͨ��
			false,//false:����ײ
			Ignores3,
			EDrawDebugTrace::Type::ForOneFrame,//ForOneFrameһ֡���� None������
			HitGroundResult,
			true))//��������
		{
			if (HitHeadResult.bBlockingHit)
			{
				GroundDistance = FVector::Distance(StartTraceLocation, HitHeadResult.Location);
			}
		}
	}

	if (HitChestResult.bBlockingHit&&HitHeadResult.bBlockingHit)
	{//��ǽ
		//if (ChestDistance<=75.f && HeadDistance<=75.f)
		if (ChestDistance <= 45.f)
		{
			//����
			float CompensationValue = ChestDistance - 28.f;
			if (CompensationValue>0.f)
			{
				FVector TargetPoint = ForwardDireaction * CompensationValue;
				FVector TargetLocation = 
					MMOARPGCharacterBase->GetActorLocation()+TargetPoint* (3.f) *DeltaTime;

				MMOARPGCharacterBase->SetActorLocation(TargetLocation);
			}
			//��ǽ
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING)
			{
				if (HitGroundResult.bBlockingHit)
				{
					if (GroundDistance<=1.f)
					{
						ClimbingState = EClimbingState::CLIMBING_TOGROUND;
						ReleaseClimbing();
					}
				}
			}
			else if (ClimbingState != EClimbingState::CLIMBING_TOGROUND)
			{
				ClimbingState = EClimbingState::CLIMBING_CLIMBING;
				Climbing();
			}
			else
			{
				if (ClimbingState==EClimbingState::CLIMBING_TOGROUND)
				{
					ClimbingState = EClimbingState::CLIMBING_NONE;
				}		
			}
			if (!HitGroundResult.bBlockingHit)
			{
				if (bJumpToClimbing)
				{
					bJumpToClimbing = false;
				}
				else
				{
					bJumpToClimbing = true;
				}
			}
			else
			{
				bJumpToClimbing = false;
			}
		}
	}
	else if (HitChestResult.bBlockingHit)
	{//��ǽ �Ƕ�
		if (ClimbingState ==EClimbingState::CLIMBING_CLIMBING)
		{
			ClimbingState = EClimbingState::CLIMBING_TOTOP;
			//Э��
			GThread::Get()->GetCoroutines().BindLambda(0.9f, [this]()
				{
					ReleaseClimbing();
				});
		}
		else if (ClimbingState != EClimbingState::CLIMBING_TOTOP
			&&ClimbingState!=EClimbingState::CLIMBING_WALLCLIMBING
			&&!bWallClimbing)
		{
			if (ChestDistance<= 26.f)
			{
				{//����ǽ
					FRotator NewRot = FRotationMatrix::MakeFromX(
						MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();
					ActorRotation.Yaw = NewRot.Yaw;
					ActorRotation.Pitch = 0.f;
					ActorRotation.Roll = 0.f;
					MMOARPGCharacterBase->SetActorRotation(ActorRotation);
				}
				//���µ�����
				FHitResult HitWallClimbing;
				FVector StartTraceLocation = LocaLocation + ForwardDireaction * 40.f;
				StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
				FVector EndTraceLocation = StartTraceLocation - UpDireaction * 100.f;
				TArray<AActor*> ClimbingActorToIgnore;

				UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					StartTraceLocation,
					EndTraceLocation,
					ETraceTypeQuery::TraceTypeQuery1,//��ѯͨ��
					false,//false:����ײ
					ClimbingActorToIgnore,
					EDrawDebugTrace::Type::ForOneFrame,//ForOneFrameһ֡���� None������
					HitWallClimbing,
					true);
				if (HitWallClimbing.bBlockingHit)
				{
					HitWallClimbing.Location.Z += HitWallClimbing.Distance;
					ClimbingTracePoint = HitWallClimbing.Location;
					//��ȡǽ��
					ClimbingHeight = HitWallClimbing.Distance;

					ClimbingState = EClimbingState::CLIMBING_WALLCLIMBING;
					bWallClimbing = true;
					bWallClimbing = 1.6f;
				}


			}
		}
	}
	else 
	{//�ͷ�
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING)
		{
			ClimbingState = EClimbingState::CLIMBING_NONE;
			ReleaseClimbing();
		}	
	}

	if (HitChestResult.bBlockingHit)
	{
		//ֻ������״̬�Ż�ı�
		if (ClimbingState==EClimbingState::CLIMBING_CLIMBING)
		{
			FRotator NewRot = FRotationMatrix::MakeFromX(
				MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();

			ActorRotation.Yaw = NewRot.Yaw;
			ActorRotation.Pitch = NewRot.Pitch;
			ActorRotation.Roll = 0.f;

			MMOARPGCharacterBase->SetActorRotation(ActorRotation);
		}

	}

	if (bWallClimbing)
	{//�ƶ������ߵ�
		FVector VInterpTolocation = FMath::VInterpTo(LocaLocation, ClimbingTracePoint,DeltaTime,7.f);
		MMOARPGCharacterBase->SetActorLocation(VInterpTolocation);
	}

}

void UClimbingComponent::SetClimbingState(EMovementMode InMode, ECharacterActionState InCharacterActionState, bool bOrientRotation)
{

	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();
	CharacterMovementComponent->SetMovementMode(InMode);
	CharacterMovementComponent->bOrientRotationToMovement = bOrientRotation;

	MMOARPGCharacterBase->ResetActionState(InCharacterActionState);
	ActorRotation.Pitch = 0.f;
	MMOARPGCharacterBase->SetActorRotation(ActorRotation);
	bJumpToClimbing = false;
}

void UClimbingComponent::ReleaseClimbing()
{
	SetClimbingState(EMovementMode::MOVE_Walking, ECharacterActionState::NORMAL_STATE,true);
}

void UClimbingComponent::Climbing()
{
	SetClimbingState(EMovementMode::MOVE_Custom, ECharacterActionState::CLIMB_STATE, false);
}

void UClimbingComponent::ClearClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_NONE;
}

bool UClimbingComponent::IsLowClimbing()
{
	return ClimbingHeight <= 40.f;
}
