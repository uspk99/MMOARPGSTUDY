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
	bFastFly = false;
	// ...
}


// Called when the game starts
void UFlyComponent::BeginPlay()
{
	Super::BeginPlay();
	MMOARPGCharacterBase = Cast<AMMOARPGCharacterBase>(GetOwner());
	if (MMOARPGCharacterBase.IsValid())
	{
		CharacterMovementComponent = Cast<UCharacterMovementComponent>(MMOARPGCharacterBase->GetMovementComponent());
		CapsuleComponent = (MMOARPGCharacterBase->GetCapsuleComponent());
		CameraComponent = MMOARPGCharacterBase->GetFollowCamera();
	}

	// ...

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
			//��ɫ��ת
			{
				//����ͽ�������ת
				FRotator CameraRotator = CameraComponent->GetComponentRotation();
				FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();
				//����Pitch
				if (!bFastFly)
				{
					CameraRotator.Pitch = 0.f;
				}
				//�����ٶ�ת��
				FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator,DeltaTime,8.f);
				MMOARPGCharacterBase->SetActorRotation(NewRot);
			}		
			{
				if (1)
				{//֡
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
				{//���ٶ�
					//����pitch yaw roll
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
			CharacterMovementComponent->MaxFlySpeed = 600.f;
			CharacterMovementComponent->bOrientRotationToMovement = true;
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);

			//�ص�������
			FRotator NewRot = MMOARPGCharacterBase->GetActorRotation();
			NewRot.Pitch = 0.f;
			MMOARPGCharacterBase->SetActorRotation(NewRot);
		}
		
		bFastFly = false;
	}
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

void UFlyComponent::FlyForwardAxis(float InAxisValue)
{
	if (CharacterMovementComponent.IsValid() &&
		MMOARPGCharacterBase.IsValid() &&
		CameraComponent.IsValid() &&
		CapsuleComponent.IsValid())
	{

		if (bFastFly)
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