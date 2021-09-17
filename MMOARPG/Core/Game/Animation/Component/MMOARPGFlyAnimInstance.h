// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "../../../../MMOARPGGameType.h"
#include "Core/AnimInstanceComponentBase.h"
#include "MMOARPGFlyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UMMOARPGFlyAnimInstance : public UAnimInstanceComponentBase
{
	GENERATED_BODY()
		UMMOARPGFlyAnimInstance();

		virtual void InitAnimInstance(ACharacter* InCharacter);

	//初始化与更新
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
public:



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bLand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EDodgeFly DodgeFly;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bFast;

};
