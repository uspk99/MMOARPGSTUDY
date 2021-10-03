#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystemComponent.h"


UMMOARPGGameplayAbility:: UMMOARPGGameplayAbility()
{

}

int32 UMMOARPGGameplayAbility::GetCompositeSectionsNumber()
{
	//��ȡ��̫��Ƭ������
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
		//ע��Ŀ����Ϣ
			FGameplayAbilityTargetData_ActorArray* NewTargetData_ActorArray
				=new FGameplayAbilityTargetData_ActorArray();
				//ȥconst
			NewTargetData_ActorArray->TargetActorArray.Add(const_cast<AActor*>(Payload.Target));
			GameplayEffectSpec.TargetHandleData.Add(NewTargetData_ActorArray);
		
		//ע��Ч��
			for (auto& Tmp : InEffect->TargetEffectClasses)
			{//�õ�ע����Ҫ��ʵ��
				FGameplayEffectSpecHandle NewHandle=			
				GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec
					(Tmp,
					1,//�ȼ�
					MakeEffectContext(CurrentSpecHandle,CurrentActorInfo));//�����ģ���ǰʵ���������ǰ��ɫ��Ϣ��
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
		AbilityTags,//����Tags
		Rate, //����
		StartSection, //��ʼ��12345����
		bStopWhenAbilityEnds, 
		AnimRootMotionTranslationScale,//RootMotion�任�ߴ� 
		StartTimeSeconds)//��ʼʱ��
		)
	{
		InWait->OnBlendOut.AddDynamic(this, &UMMOARPGGameplayAbility::OnBlendOut);
		InWait->OnCancelled.AddDynamic(this, &UMMOARPGGameplayAbility::OnCancelled);
		InWait->OnCompleted.AddDynamic(this, &UMMOARPGGameplayAbility::OnCompleted);
		InWait->OnInterrupted.AddDynamic(this, &UMMOARPGGameplayAbility::OnInterrupted);
		//
		InWait->DamageEventReceived.AddDynamic(this, &UMMOARPGGameplayAbility::OnDamageGameplayEvent);
		//����
		InWait->Activate();

		return InWait;
	}
	return nullptr;
}

UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::PlayMontage(FName StartSection)
{
	return CreatePlayMontageAndWaitProxy(NAME_None, MontageToPlay,1.f,StartSection);
}
