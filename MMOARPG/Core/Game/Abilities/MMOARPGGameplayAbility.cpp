#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"


UMMOARPGGameplayAbility:: UMMOARPGGameplayAbility()
{

}

int32 UMMOARPGGameplayAbility::GetCompositeSectionsNumber()
{
	//获取蒙太奇片段数量
	if (MontageToPlay)
	{
		return MontageToPlay->CompositeSections.Num();
	}
	return 0;
}

void UMMOARPGGameplayAbility::OnCompleted()
{
	K2_OnCompleted();
}

void UMMOARPGGameplayAbility::OnBlendOut()
{
	K2_OnBlendOut();
}

void UMMOARPGGameplayAbility::OnInterrupted()
{
	K2_OnInterrupted();
}

void UMMOARPGGameplayAbility::OnCancelled()
{
	K2_OnCancelled();
}

void UMMOARPGGameplayAbility::OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData Payload)
{
	if (FMMOARPGGameplayEffects* InEffect = EffectMap.Find(InGameplayTag))
	{
		FMMOARPGGameplayEffectSpec GameplayEffectSpec;
		{
		//注册目标信息
			FGameplayAbilityTargetData_ActorArray* NewTargetData_ActorArray
				=new FGameplayAbilityTargetData_ActorArray();
				//去const
			NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(Payload.Target));
			GameplayEffectSpec.TargetHandleData.Add(NewTargetData_ActorArray);
		
		//注册效果
			for (auto& Tmp : InEffect->TargetEffectClasses)
			{//拿到注册需要的实例
				FGameplayEffectSpecHandle NewHandle=			
				GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
					(Tmp,
					1,//等级
					MakeEffectContext(CurrentSpecHandle,CurrentActorInfo));//上下文（当前实例句柄，当前角色信息）
				if (NewHandle.IsValid())
				{
					FGameplayAbilitySpec* AbilitySpec 
						= GetAbilitySystemComponentFromActorInfo()->FindAbilitySpecFromHandle(CurrentSpecHandle);

					ApplyAbilityTagsToGameplayEffectSpec(*NewHandle.Data.Get(),AbilitySpec);

					if (AbilitySpec)
					{
						NewHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
					}
				}
				GameplayEffectSpec.TargetEffectSpecs.Add(NewHandle);
			}
		}

		for (auto &Tmp:GameplayEffectSpec.TargetEffectSpecs)
		{
			TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandle
				=K2_ApplyGameplayEffectSpecToTarget(Tmp,GameplayEffectSpec.TargetHandleData);
		}

	}
}

UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::CreatePlayMontageAndWaitProxy(FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	if (UAbilityTask_PNAWDamageEvent* InWait= UAbilityTask_PNAWDamageEvent::CreatePMAWDamageProxy(
		this,
		TaskInstanceName, 
		InMontageToPlay, 
		AbilityTags,//传入Tags
		Rate, //速率
		StartSection, //开始的12345播放
		bStopWhenAbilityEnds, 
		AnimRootMotionTranslationScale,//RootMotion变换尺寸 
		StartTimeSeconds)//开始时间
		)
	{
		InWait->OnBlendOut.AddDynamic(this, &UMMOARPGGameplayAbility::OnBlendOut);
		InWait->OnCancelled.AddDynamic(this, &UMMOARPGGameplayAbility::OnCancelled);
		InWait->OnCompleted.AddDynamic(this, &UMMOARPGGameplayAbility::OnCompleted);
		InWait->OnInterrupted.AddDynamic(this, &UMMOARPGGameplayAbility::OnInterrupted);
		//
		InWait->DamageEventReceived.AddDynamic(this, &UMMOARPGGameplayAbility::OnDamageGameplayEvent);
		//激活
		InWait->Activate();

		return InWait;
	}
	return nullptr;
}

UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::PlayMontage(FName StartSection)
{
	return CreatePlayMontageAndWaitProxy(NAME_None, MontageToPlay,1.f,StartSection);
}
