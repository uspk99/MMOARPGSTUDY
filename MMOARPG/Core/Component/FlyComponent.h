// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "../../MMOARPGGameType.h"
#include "Core/MotionComponent.h"
#include "FlyComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UFlyComponent : public UMotionComponent
{
	GENERATED_BODY()

public:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	//	bool bFast;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EDodgeFly DodgeFly;

		FResultBool bLand;
public:	
	// Sets default values for this component's properties
	UFlyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Reset();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FlyForwardAxis(float InAxisValue);
	void ResetFly();
	void ResetFastFly();

	void ResetDodgeFly(EDodgeFly InDodgeFly);

	UFUNCTION()
	void Landed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	//UPROPERTY()
	//float DodgeTime;
};
