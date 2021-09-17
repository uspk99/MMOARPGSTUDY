// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Instance/Core/MMOARPGAnimInstanceBase.h"
#include "AnimInstanceComponentBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UAnimInstanceComponentBase : public UMMOARPGAnimInstanceBase
{
	GENERATED_BODY()

public:
		UAnimInstanceComponentBase();
		virtual void InitAnimInstance(ACharacter* InCharacter);

	//��ʼ�������
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector AxisSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;
protected:
	void ResetAxisSpeed(float NewSpeed);
};
