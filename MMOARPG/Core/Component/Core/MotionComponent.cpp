// Fill out your copyright notice in the Description page of Project Settings.


#include "MotionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../../Game/Character/Core/MMOARPGCharacterBase.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UMotionComponent::UMotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotionComponent::BeginPlay()
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


// Called every frame
void UMotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//Dodge飞行计时,并重置
	//bFast.Tick(DeltaTime);
}



void UMotionComponent::LockView(float DeltaTime, bool bClearPitch /*= false*/)
{
	//相机和胶囊体旋转
	FRotator CameraRotator = CameraComponent->GetComponentRotation();
	FRotator CapsuleRotator = CapsuleComponent->GetComponentRotation();
	//修正Pitch
	if (!bClearPitch)
	{
		CameraRotator.Pitch = 0.f;
	}
	//根据速度转向
	FRotator NewRot = FMath::RInterpTo(CapsuleRotator, CameraRotator, DeltaTime, 8.f);
	MMOARPGCharacterBase->SetActorRotation(NewRot);
}

void UMotionComponent::ResetRotationRate(float DeltaTime)
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

		RotationRate.Y = FMath::GetMappedRangeValueClamped(
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

void UMotionComponent::Print(float InTime, const FString& InString)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, InTime, FColor::Red, FString::Printf(TEXT("%s"), *InString));
	}
}