// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "MMOARPG/MMOARPGGameType.h"
#include "MMOARPGAnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UMMOARPGAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMMOARPGAnimInstanceBase();

	virtual void InitAnimInstance(ACharacter* InCharacter);

	//初始化与更新
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AnimAttrubute")
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bInAir;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	//	bool bFight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	ECharacterActionState ActionState;
public:
	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
		bool bFootIK;
	//左右脚，Z偏移
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	float LeftOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		float RightOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
		float ButtZOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute|FootIK")
	int32 FootIKID;

	UPROPERTY(EditDefaultsOnly, Category = "AnimAttrubute|FootIK")
	TArray<FName> BoneNames;

	//两足
	UPROPERTY(EditDefaultsOnly,Category = "AnimAttrubute|FootIK")
		FName LeftBoneName;

	UPROPERTY(EditDefaultsOnly,Category = "AnimAttrubute|FootIK")
		FName RightBoneName;

public:
	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetFootIKOffset(const FName& InBoneName);
};
