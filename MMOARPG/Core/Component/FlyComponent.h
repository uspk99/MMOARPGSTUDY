// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "FlyComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UFlyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFastFly;
public:	
	// Sets default values for this component's properties
	UFlyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Print(float InTime, const FString& InString);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FlyForwardAxis(float InAxisValue);
	void ResetFly();
	void ResetFastFly();
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
