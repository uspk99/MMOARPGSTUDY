// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "Camera/CameraComponent.h"
// Sets default values for this component's properties
UFlyComponent::UFlyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//bFastFly = false;
	//DodgeTime = 0.f;
	// ...
}


// Called when the game starts
void UFlyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MMOARPGCharacterBase.IsValid())
	{
		if (CharacterMovementComponent.IsValid())
		{
			//最大加速度
			CharacterMovementComponent->MaxAcceleration = 2500.f;
			//飞行减速
			CharacterMovementComponent->BrakingDecelerationFlying = 1400.f;
		}
		//落地代理
		//MMOARPGCharacterBase->LandedDelegate.AddDynamic(this, &UFlyComponent::Landed);
		if (CapsuleComponent.IsValid())
		{
			CapsuleComponent->OnComponentHit.AddDynamic(this, &UFlyComponent::Landed);
		}
		bFastFly.Fun.BindLambda([&]() { DodgeFly = EDodgeFly::DODGE_NONE; });
		//登陆之后回到地面
		bLand.Fun.BindLambda([&]()
			{
				if (MMOARPGCharacterBase.IsValid())
				{
					MMOARPGCharacterBase->ResetActionState(ECharacterActionState::FLIGHT_STATE);
					ResetFly();
				}});
	}
}

void UFlyComponent::Landed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (MMOARPGCharacterBase.IsValid())
	{
		if (MMOARPGCharacterBase->GetActionState()==ECharacterActionState::FLIGHT_STATE
			&&bFastFly)
		{
			//点乘
			float CosValue =
			FVector::DotProduct(CapsuleComponent->GetForwardVector(),Hit.ImpactNormal);
			
			float CosAngle = (180.f)/PI* FMath::Acos(CosValue);
			//角度
			if (CosAngle>=125.f)
			{
				//是否地面而非山脉
				if (Hit.ImpactNormal.Z>0.5f)
				{
					Reset();
					bLand = true;
					bLand = 1.6f;
				}
			}	
		}
	}
}

void UFlyComponent::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}

// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (CharacterMovementComponent.IsValid()&&
		MMOARPGCharacterBase.IsValid()&&
		CameraComponent.IsValid()&&
		CapsuleComponent.IsValid())
	{
		if (MMOARPGCharacterBase->GetActionState() == ECharacterActionState::FLIGHT_STATE)
		{
			//模拟玩家不计算
			if (MMOARPGCharacterBase->GetLocalRole()==ENetRole::ROLE_Authority||
				MMOARPGCharacterBase->GetLocalRole()==ENetRole::ROLE_AutonomousProxy)
			{
				if (!bLand)
				{
					//角色旋转
					{
						//相机和胶囊体旋转
						FRotator CameraRotator = CameraComponent->GetComponentRotation();
						FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();
						//修正Pitch
						if (!bFastFly)
						{
							CameraRotator.Pitch = 0.f;
						}
						//根据速度转向
						FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator,DeltaTime,8.f);
						MMOARPGCharacterBase->SetActorRotation(NewRot);
					}		
					{
						if (1)
						{//帧
							float PreFPS = 1.f / DeltaTime;
							FRotator NewDeltaTimeRot = MMOARPGCharacterBase->GetActorRotation() - LastRotator;
							NewDeltaTimeRot *= PreFPS;

							FVector PhysicsAngularVelocityInDegrees;
							Print(DeltaTime, PhysicsAngularVelocityInDegrees.ToString());
							RotationRate.X = FMath::GetMappedRangeValueClamped(
								FVector2D(-360.f, 360.f),
								FVector2D(-1.f, 1.f),
								NewDeltaTimeRot.Yaw);

							RotationRate.Y= FMath::GetMappedRangeValueClamped(
								FVector2D(-360.f, 360.f),
								FVector2D(-1.f, 1.f),
								NewDeltaTimeRot.Pitch);

							LastRotator = MMOARPGCharacterBase->GetActorRotation();
						}
						else
						{//角速度
							//返回pitch yaw roll
							FVector PhysicsAngularVelocityInDegrees = CapsuleComponent->GetPhysicsAngularVelocityInDegrees();
							RotationRate.X = FMath::GetMappedRangeValueClamped(
								 FVector2D(-360.f, 360.f),
								 FVector2D(-1.f, 1.f), 
								 PhysicsAngularVelocityInDegrees.Z);

							RotationRate.Y = FMath::GetMappedRangeValueClamped(
								FVector2D(-360.f, 360.f),
								FVector2D(-1.f, 1.f),
								PhysicsAngularVelocityInDegrees.Y);
						}
					}
				}

			}

		}
		
		//Dodge飞行计时,并重置
		bFastFly.Tick(DeltaTime);

		//着地计时
		bLand.Tick(DeltaTime);
	}
}

void UFlyComponent::ResetFly()
{
	if (CharacterMovementComponent.IsValid() &&MMOARPGCharacterBase.IsValid())
	{
		if (MMOARPGCharacterBase->GetActionState()==ECharacterActionState::FLIGHT_STATE)
		{
			//更改旋转设定
			CharacterMovementComponent->bOrientRotationToMovement = false;
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		}
		else
		{
			Reset();
		}
		
		bFastFly = false;
	}
}
void UFlyComponent::Reset()
{

	CharacterMovementComponent->MaxFlySpeed = 600.f;
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);

	//回调俯仰角
	FRotator NewRot = MMOARPGCharacterBase->GetActorRotation();
	NewRot.Pitch = 0.f;
	MMOARPGCharacterBase->SetActorRotation(NewRot);

}

void UFlyComponent::ResetFastFly()
{
	if (CharacterMovementComponent.IsValid())
	{
		if (bFastFly)
		{
			bFastFly = false;
			CharacterMovementComponent->MaxFlySpeed = 600.f;
		}
		else
		{
			bFastFly = true;
			CharacterMovementComponent->MaxFlySpeed = 2000.f;
		}
	}

}

void UFlyComponent::ResetDodgeFly(EDodgeFly InDodgeFly)
{
	if (bFastFly)
	{
		bFastFly = 1.6f;
		DodgeFly = InDodgeFly;
		//DodgeTime = 1.6f;
	}
}


void UFlyComponent::FlyForwardAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{

		if (bFastFly)
		{
			//按下 直接加速向前
			const FVector Direction = CameraComponent->GetForwardVector();
			MMOARPGCharacterBase->AddMovementInput(Direction, 1.f);
		}
		else
		{
			const FVector Direction = CameraComponent->GetForwardVector();
			MMOARPGCharacterBase->AddMovementInput(Direction, InAxisValue);
		}


	}
}