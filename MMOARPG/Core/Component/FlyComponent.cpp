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
	//bFast = false;
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
		bFast.Fun.BindLambda([&]() { DodgeFly = EDodgeFly::DODGE_NONE; });
		//登陆之后回到地面
		bLand.Fun.BindLambda([&]()
			{
				if (MMOARPGCharacterBase.IsValid())
				{
					MMOARPGCharacterBase->ResetActionState(ECharacterActionState::NORMAL_STATE);
					ResetFly();
				}});

		bDodgeFly.Fun.BindLambda([&]() {DodgeFly = EDodgeFly::DODGE_NONE; });
	}
}

void UFlyComponent::Landed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (MMOARPGCharacterBase.IsValid())
	{
		if (MMOARPGCharacterBase->GetActionState()==ECharacterActionState::FLIGHT_STATE
			&&bFast)
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


// Called every frame
void UFlyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	bFast.Tick(DeltaTime,false);
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
					LockView(DeltaTime,*bFast);
					ResetRotationRate(DeltaTime);	
				}
			}
		}
		//着地计时
		bLand.Tick(DeltaTime,true);
		bDodgeFly.Tick(DeltaTime, true);
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
		
		bFast = false;
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

	NewRot.Roll = 0.f;

	MMOARPGCharacterBase->SetActorRotation(NewRot);

}

void UFlyComponent::ResetFastFly()
{
	if (CharacterMovementComponent.IsValid())
	{
		if (bFast)
		{
			bFast = false;
			CharacterMovementComponent->MaxFlySpeed = 600.f;
		}
		else
		{
			bFast = true;
			CharacterMovementComponent->MaxFlySpeed = 2000.f;
		}
	}

}

void UFlyComponent::ResetDodgeFly(EDodgeFly InDodgeFly)
{
	if (bFast)
	{
		bFast = 1.6f;
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

		if (bFast)
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