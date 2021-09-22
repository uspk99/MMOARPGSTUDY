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
			//�����ٶ�
			CharacterMovementComponent->MaxAcceleration = 2500.f;
			//���м���
			CharacterMovementComponent->BrakingDecelerationFlying = 1400.f;
		}
		//��ش���
		//MMOARPGCharacterBase->LandedDelegate.AddDynamic(this, &UFlyComponent::Landed);
		if (CapsuleComponent.IsValid())
		{
			CapsuleComponent->OnComponentHit.AddDynamic(this, &UFlyComponent::Landed);
		}
		bFast.Fun.BindLambda([&]() { DodgeFly = EDodgeFly::DODGE_NONE; });
		//��½֮��ص�����
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
			//���
			float CosValue =
			FVector::DotProduct(CapsuleComponent->GetForwardVector(),Hit.ImpactNormal);
			
			float CosAngle = (180.f)/PI* FMath::Acos(CosValue);
			//�Ƕ�
			if (CosAngle>=125.f)
			{
				//�Ƿ�������ɽ��
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
			//ģ����Ҳ�����
			if (MMOARPGCharacterBase->GetLocalRole()==ENetRole::ROLE_Authority||
				MMOARPGCharacterBase->GetLocalRole()==ENetRole::ROLE_AutonomousProxy)
			{
				if (!bLand)
				{
					//��ɫ��ת			
					LockView(DeltaTime,*bFast);
					ResetRotationRate(DeltaTime);	
				}
			}
		}
		//�ŵؼ�ʱ
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
			//������ת�趨
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

	//�ص�������
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
			//���� ֱ�Ӽ�����ǰ
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