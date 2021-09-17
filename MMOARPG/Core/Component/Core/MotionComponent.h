// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MotionComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UMotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMotionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
		TWeakObjectPtr<AMMOARPGCharacterBase> MMOARPGCharacterBase;
	UPROPERTY()
		TWeakObjectPtr<UCharacterMovementComponent>  CharacterMovementComponent;
	UPROPERTY()
		TWeakObjectPtr<UCapsuleComponent>  CapsuleComponent;
	UPROPERTY()
		TWeakObjectPtr<UCameraComponent>  CameraComponent;

	UPROPERTY()
		FRotator LastRotator;
};
