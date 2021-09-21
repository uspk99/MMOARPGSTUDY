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
	
	bWallClimbing.Fun.BindLambda([this]()
		{
			AdjustmentClimbing(false);
		});

	bTurn.Fun.BindLambda([this]()
		{
			CharacterMovementComponent->GravityScale = 1.f;

			FVector ForwardDireaction = MMOARPGCharacterBase->GetActorForwardVector();
			FVector LocaLocation = MMOARPGCharacterBase->GetActorLocation();

			FHitResult HitChestResult;
			float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
				{
					StartTraceLocation = LocaLocation;
					StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.f;
					EndTraceLocation = StartTraceLocation +
						ForwardDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
				});
			//修正 Turn
			if (ChestDistance>=29.f)
			{
				LocaLocation += ForwardDireaction * (ChestDistance - 29.f);
			}
			else
			{
				LocaLocation -= ForwardDireaction * ( 29.f-ChestDistance );
			}
			MMOARPGCharacterBase->SetActorLocation(LocaLocation);
		});
}

void UClimbingComponent::LaunchCharacter(const FVector InVelocity)
{
	PendingLaunchVelocity = InVelocity;
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
	bTurn.Tick(DeltaTime, true);

	AdjustmentPendingLaunchVelocity(DeltaTime);
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
			//攀爬方向
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
			//拿到ACtor自身的旋转
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
		{//小于最小时间，无移动必要
			return;
		}
		if (!CharacterMovementComponent->HasAnimRootMotion())
		{
			//更新Velocity last方向乘以速度
			//添加自定义力
			CharacterMovementComponent->Velocity = PendingLaunchVelocity + CharacterMovementComponent->GetLastInputVector()
				* CharacterMovementComponent->MaxCustomMovementSpeed;
		}
		else
		{
			//动画的rootMotion
			CharacterMovementComponent->Velocity = PendingLaunchVelocity +
				CharacterMovementComponent->ConstrainAnimRootMotionVelocity(
					CharacterMovementComponent->AnimRootMotionVelocity,
					CharacterMovementComponent->Velocity);
		}

		//每帧调节
		const FVector Adjusted = CharacterMovementComponent->Velocity * deltaTime;

		FHitResult Hit(1.f);
		//GetComponentQuat返回旋转 true是扫描 碰撞结果
		CharacterMovementComponent->SafeMoveUpdatedComponent
		(Adjusted,CharacterMovementComponent->UpdatedComponent->GetComponentQuat(),true,Hit);
		
	}


}

void UClimbingComponent::TraceClimbingState(float DeltaTime)
{
	//转向时停止检测
	if (bTurn)
	{
		return;
	}

	FVector ForwardDireaction = MMOARPGCharacterBase->GetActorForwardVector();
	FVector UpDireaction = MMOARPGCharacterBase->GetActorUpVector();
	FVector LocaLocation = MMOARPGCharacterBase->GetActorLocation();
	FVector RightDireaction = MMOARPGCharacterBase->GetActorRightVector();

	FRotator ActorRotation = MMOARPGCharacterBase->GetActorRotation();
	

	FHitResult HitChestResult;
	float ChestDistance = Scanning(HitChestResult, [&](FVector& StartTraceLocation,FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation;
			StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight() / 2.f;
			EndTraceLocation = StartTraceLocation + 
				ForwardDireaction*CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
		});

	FHitResult HitHeadResult;
	float HeadDistance = Scanning(HitHeadResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation;
			StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
			EndTraceLocation = StartTraceLocation + 
				ForwardDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight() * 2.f;
		});

	FHitResult HitGroundResult;
	float GroundDistance = Scanning(HitGroundResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation;
			StartTraceLocation.Z -= CapsuleComponent->GetScaledCapsuleHalfHeight();
			EndTraceLocation = StartTraceLocation +
				(-UpDireaction) * 40.f;
		});
	FHitResult HitRightResult;
	float RightDistance = Scanning(HitRightResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation + RightDireaction * 70.f;
			EndTraceLocation = StartTraceLocation + ForwardDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	FHitResult HitLeftResult;
	float LeftDistance = Scanning(HitLeftResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation - RightDireaction * 70.f;
			EndTraceLocation = StartTraceLocation + ForwardDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});
	FHitResult HitLeftSideResult;
	float LeftSideDistance = Scanning(HitLeftSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation - RightDireaction * 10.f-ForwardDireaction*10.f;
			EndTraceLocation = StartTraceLocation - RightDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	FHitResult HitRightSideResult;
	float RightSideDistance = Scanning(HitRightSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
		{
			StartTraceLocation = LocaLocation + RightDireaction * 10.f - ForwardDireaction * 10.f;
			EndTraceLocation = StartTraceLocation + RightDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight();
		});

	auto CheckTurn = [&]()
	{
		if (!HitRightResult.bBlockingHit && !HitLeftSideResult.bBlockingHit && !HitRightSideResult.bBlockingHit)
		{
			FHitResult HitRightClimbingSideResult;
			float RightClimbingSideDistance = Scanning(HitRightClimbingSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
				{
					StartTraceLocation = LocaLocation + RightDireaction * 70.f + ForwardDireaction * 40.f;
					EndTraceLocation = StartTraceLocation - RightDireaction * 100.f;
				});
			if (HitRightClimbingSideResult.bBlockingHit&&RightClimbingSideDistance!=0.f)
			{
				if (RightClimbingSideDistance>=8.f)
				{
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbTurnState::OUTSIDE_RIGHT;
				}
			}
		}
		else if (!HitLeftResult.bBlockingHit && !HitLeftSideResult.bBlockingHit && !HitRightSideResult.bBlockingHit)
		{
			FHitResult HitLeftClimbingSideResult;
			float LeftClimbingSideDistance = Scanning(HitLeftClimbingSideResult, [&](FVector& StartTraceLocation, FVector& EndTraceLocation)
				{
					StartTraceLocation = LocaLocation - RightDireaction * 70.f + ForwardDireaction * 40.f;
					EndTraceLocation = StartTraceLocation + RightDireaction * 100.f;
				});
			if (HitLeftClimbingSideResult.bBlockingHit && LeftClimbingSideDistance != 0.f)
			{
				if (LeftClimbingSideDistance >= 8.f)
				{
					bTurn = true;
					bTurn = 1.2f;
					TurnState = EClimbTurnState::OUTSIDE_LEFT;
				}
			}
		}
		else if (HitLeftSideResult.bBlockingHit)
		{//转弯临界值
			if (FMath::IsNearlyEqual(LeftSideDistance, 43.f, 0.7f))
			{
				bTurn = true;
				bTurn = 1.2f;
				TurnState = EClimbTurnState::INSIDE_LEFT;

			}
		}
		else if (HitRightSideResult.bBlockingHit)
		{//转弯临界值不同，防止反复横跳
			if (FMath::IsNearlyEqual(RightSideDistance,44.f,0.7f))
			{
				bTurn = true;
				bTurn = 1.2f;
				TurnState = EClimbTurnState::INSIDE_RIGHT;

			}
		}
		if (bTurn)
		{
			ClimbingState = EClimbingState::CLIMBING_TRUN;
			CharacterMovementComponent->GravityScale = 0.f;
		}

	};

	if (HitChestResult.bBlockingHit&&HitHeadResult.bBlockingHit)
	{//爬墙
		//if (ChestDistance<=75.f && HeadDistance<=75.f)
		if (ChestDistance <= 45.f)
		{
			//贴近
			float CompensationValue = ChestDistance - 28.f;
			if (CompensationValue>0.f && ClimbingState!=EClimbingState::CLIMBING_DROP)
			{
				FVector TargetPoint = ForwardDireaction * CompensationValue;
				FVector TargetLocation = 
					MMOARPGCharacterBase->GetActorLocation()+TargetPoint* (3.f) *DeltaTime;

				MMOARPGCharacterBase->SetActorLocation(TargetLocation);
			}
			//爬墙
			if (ClimbingState == EClimbingState::CLIMBING_CLIMBING)
			{
				if (HitGroundResult.bBlockingHit)
				{
					if (GroundDistance<=1.f)
					{
						ClimbingState = EClimbingState::CLIMBING_TOGROUND;
						ReleaseClimbing();
					}
					//检测是否旋转攀爬 近地
					else if (
						HitRightResult.bBlockingHit ||
						HitLeftResult.bBlockingHit ||
						HitLeftSideResult.bBlockingHit ||
						HitRightSideResult.bBlockingHit)
					{
						if (!bTurn)
						{
							CheckTurn();
						}
					}
				}
				//检测是否旋转攀爬 高处
				else if (
					HitRightResult.bBlockingHit ||
					HitLeftResult.bBlockingHit ||
					HitLeftSideResult.bBlockingHit ||
					HitRightSideResult.bBlockingHit)
				{
					if (!bTurn)
					{
						CheckTurn();
					}
				}
				//角度调整
				FVector ZAxis = FVector(0.f,0.f,1.f);
				float CosValue = FVector::DotProduct(ZAxis, HitChestResult.ImpactNormal);
				float CosAngle = (180.f / PI) * FMath::Acos(CosValue);
				if (CosAngle < 35.f)
				{
					ClimbingState = EClimbingState::CLIMBING_NONE;
					ReleaseClimbing();
				}
			}
			//进入爬墙
			else if (ClimbingState != EClimbingState::CLIMBING_TOGROUND
				&&ClimbingState!=EClimbingState::CLIMBING_DROP)
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
	else if (HitChestResult.bBlockingHit && !HitHeadResult.bBlockingHit)
	{//翻墙 登顶
		if (ClimbingState ==EClimbingState::CLIMBING_CLIMBING
			&& !HitGroundResult.bBlockingHit)
		{
			ClimbingState = EClimbingState::CLIMBING_TOTOP;
			//协程
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
				{//面向墙
					FRotator NewRot = FRotationMatrix::MakeFromX(
						MMOARPGCharacterBase->GetActorForwardVector() - HitChestResult.Normal).Rotator();
					ActorRotation.Yaw = NewRot.Yaw;
					ActorRotation.Pitch = 0.f;
					ActorRotation.Roll = 0.f;
					MMOARPGCharacterBase->SetActorRotation(ActorRotation);
				}
				//向下的射线
				FHitResult HitWallClimbing;
				FVector StartTraceLocation = LocaLocation + ForwardDireaction * 40.f;
				StartTraceLocation.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
				FVector EndTraceLocation = StartTraceLocation - 
					UpDireaction * CapsuleComponent->GetScaledCapsuleHalfHeight()*2.f;
				TArray<AActor*> ClimbingActorToIgnore;

				UKismetSystemLibrary::LineTraceSingle(
					GetWorld(),
					StartTraceLocation,
					EndTraceLocation,
					ETraceTypeQuery::TraceTypeQuery1,//查询通道
					false,//false:简单碰撞
					ClimbingActorToIgnore,
					EDrawDebugTrace::Type::ForOneFrame,//ForOneFrame一帧绘制 None不绘制
					HitWallClimbing,
					true);
				if (HitWallClimbing.bBlockingHit)
				{
					HitWallClimbing.Location.Z += CapsuleComponent->GetScaledCapsuleHalfHeight();
					ClimbingTracePoint = HitWallClimbing.Location;
					//获取墙高
					ClimbingHeight = HitWallClimbing.Distance;

					ClimbingState = EClimbingState::CLIMBING_WALLCLIMBING;

					AdjustmentClimbing();
					bWallClimbing = true;
					if (IsLowClimbing())
					{
						bWallClimbing = 0.8f;
					}
					else
					{
						bWallClimbing = 1.6f;
					}
					
				}

			}
		}
	}
	else 
	{//释放
		if (ClimbingState == EClimbingState::CLIMBING_CLIMBING)
		{
			ClimbingState = EClimbingState::CLIMBING_NONE;
			ReleaseClimbing();
		}	
	}

	if (HitChestResult.bBlockingHit)
	{
		//只在爬行状态才会改变
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
	{//移动到射线点
		FVector VInterpTolocation = FMath::VInterpTo(LocaLocation, ClimbingTracePoint,DeltaTime,9.f);
		MMOARPGCharacterBase->SetActorLocation(VInterpTolocation);
	}

	if (ClimbingState == EClimbingState::CLIMBING_DROP)
	{
		if (CharacterMovementComponent->MovementMode != EMovementMode::MOVE_Falling)
		{
			ClearClimbingState();
		}
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

void UClimbingComponent::AdjustmentClimbing(bool bStart /*= true*/)
{
	FVector RelativeLocation = MMOARPGCharacterBase->GetMesh()->GetRelativeLocation();
	float AdjustValue = 10.f;
	if (bStart)
	{
		RelativeLocation.Z += AdjustValue;
	}
	else
	{
		RelativeLocation.Z -= AdjustValue;
	}
	MMOARPGCharacterBase->GetMesh()->SetRelativeLocation(RelativeLocation);
}

void UClimbingComponent::AdjustmentPendingLaunchVelocity(float DeltaTime)
{
	auto AxisCheck = [](float& InValue, float DeltaTime)
	{//与0的差距是否为1
		if (FMath::IsNearlyEqual(InValue,0.f,1.f))
		{
			InValue = 0.f;
		}
		else if (InValue>0.f)
		{
			InValue -= InValue * DeltaTime;
		}
		else if (InValue<0.f)
		{
			InValue += -InValue * DeltaTime;
		}

	};
	AxisCheck(PendingLaunchVelocity.X, DeltaTime);
	AxisCheck(PendingLaunchVelocity.Y, DeltaTime);
	AxisCheck(PendingLaunchVelocity.Z, DeltaTime);
}

float UClimbingComponent::Scanning(FHitResult& HitResult, TFunction<void(FVector&, FVector&)> TraceLocation)
{
	TArray<AActor*> Ignores1;
	float TraceDistance = MaxDistance;

	FVector StartTraceChestLocation ,EndTraceChestLocation;

	TraceLocation(StartTraceChestLocation, EndTraceChestLocation);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		StartTraceChestLocation,
		EndTraceChestLocation,
		ETraceTypeQuery::TraceTypeQuery1,//查询通道
		false,//false:简单碰撞
		Ignores1,
		EDrawDebugTrace::Type::ForOneFrame,//ForOneFrame一帧绘制 None不绘制
		HitResult,
		true);//忽略自身
	if (HitResult.bBlockingHit)
	{
		TraceDistance = FVector::Distance(StartTraceChestLocation, HitResult.Location);
	}
	return TraceDistance;
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
	return ClimbingHeight > 30.f;
}

void UClimbingComponent::DropClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_DROP;
}

bool UClimbingComponent::IsDropClimbingState()
{
	return ClimbingState == EClimbingState::CLIMBING_DROP;
}

void UClimbingComponent::ResetClimbingState()
{
	ClimbingState = EClimbingState::CLIMBING_CLIMBING;
}