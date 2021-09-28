// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/AnimInstanceComponentBase.h"
#include "../../../../MMOARPGGameType.h"
#include "MMOARPGClimbingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UMMOARPGClimbingAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
	
	virtual void InitAnimInstance(ACharacter* InCharacter);

	//初始化与更新
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFast;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bWalkToClimbing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJump;
	EClimbingState ClimbingState;

private:
	void ClimbJump();
	//计算方向
	EClimbingMontageState CalculationClimbingJumpState();
};
