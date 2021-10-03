// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilityTask_PMAWDamageEvent.generated.h"

//дһ���������������Ķಥ
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMontageWaitSimpleForDamageDelegate,FGameplayTag, EventTag, FGameplayEventData, EventData);
/** Ability task to simply play a montage. Many games will want to make a modified version of this task that looks for game-specific events */
UCLASS()
class MMOARPG_API UAbilityTask_PNAWDamageEvent : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()
public:
	UAbilityTask_PNAWDamageEvent(const FObjectInitializer& ObjectInitializer);

	FMontageWaitSimpleForDamageDelegate DamageEventReceived;
	//�����ɾ��
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;
	//�Ӳ�����ͼ
		static UAbilityTask_PNAWDamageEvent* CreatePMAWDamageProxy(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			UAnimMontage* MontageToPlay, 
			FGameplayTagContainer InEventTags,//����������Tag
			float Rate = 1.f, 
			FName StartSection = NAME_None, 
			bool bStopWhenAbilityEnds = true, 
			float AnimRootMotionTranslationScale = 1.f, 
			float StartTimeSeconds = 0.f);

private:
	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload);
	//ע���ǩ
	UPROPERTY()
	FGameplayTagContainer EventTags;

private:
	FDelegateHandle DamageEventHandle;
};