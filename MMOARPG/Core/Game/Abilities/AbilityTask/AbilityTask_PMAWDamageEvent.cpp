#include "AbilityTask_PMAWDamageEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UAbilityTask_PNAWDamageEvent::UAbilityTask_PNAWDamageEvent(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{

}

void UAbilityTask_PNAWDamageEvent::Activate()
{

	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayedMontage = false;

	if (AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
		//�󶨴���
			DamageEventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(
			EventTags,
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this,&UAbilityTask_PNAWDamageEvent::OnDamageGameplayEvent));
		}
	}
	Super::Activate();
}

void UAbilityTask_PNAWDamageEvent::OnDestroy(bool AbilityEnded)
{
	if (AbilitySystemComponent)
	{
	//�Ƴ�����
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags,DamageEventHandle);
	}
	Super::OnDestroy(AbilityEnded);
}

UAbilityTask_PNAWDamageEvent* UAbilityTask_PNAWDamageEvent::CreatePMAWDamageProxy(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer InEventTags,/*����������Tag */ float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UAbilityTask_PNAWDamageEvent* MyObj = NewAbilityTask<UAbilityTask_PNAWDamageEvent>(OwningAbility, TaskInstanceName);
	MyObj->MontageToPlay = MontageToPlay;
	MyObj->Rate = Rate;
	MyObj->StartSection = StartSection;
	MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;
	MyObj->StartTimeSeconds = StartTimeSeconds;

	//����Copy�������������ı�ǩ
	MyObj->EventTags=InEventTags;

	return MyObj;

}

void UAbilityTask_PNAWDamageEvent::OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData* Payload)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData EventData =*Payload;
		EventData.EventTag = InGameplayTag;

		DamageEventReceived.Broadcast(InGameplayTag, EventData);
	}
}
