// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "SwimmingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API USwimmingComponent : public UMotionComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bFastSwim;

	FResultBool bDiving;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SwimForwardAxis(float InAxisValue);

	void ResetFastSwimming();

	void GoUnderWater();

	
};
