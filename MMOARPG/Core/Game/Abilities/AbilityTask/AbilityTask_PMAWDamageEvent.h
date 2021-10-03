// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_PMAWDamageEvent.generated.h"

//写一个传递两个参数的多播
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMontageWaitSimpleForDamageDelegate,FGameplayTag, EventTag, FGameplayEventData, EventData);
/** Ability task to simply play a montage. Many games will want to make a modified version of this task that looks for game-specific events */
UCLASS()
class MMOARPG_API UAbilityTask_PNAWDamageEvent : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
public:
	UAbilityTask_PNAWDamageEvent(const FObjectInitializer& ObjectInitializer);

	FMontageWaitSimpleForDamageDelegate DamageEventReceived;
	//激活和删除
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;
	//加不到蓝图
		static UAbilityTask_PNAWDamageEvent* CreatePMAWDamageProxy(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			UAnimMontage* MontageToPlay, 
			FGameplayTagContainer InEventTags,//新增可输入Tag
			float Rate = 1.f, 
			FName StartSection = NAME_None, 
			bool bStopWhenAbilityEnds = true, 
			float AnimRootMotionTranslationScale = 1.f, 
			float StartTimeSeconds = 0.f);

private:
	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload);
	//注册标签
	UPROPERTY()
	FGameplayTagContainer EventTags;

private:
	FDelegateHandle DamageEventHandle;
};