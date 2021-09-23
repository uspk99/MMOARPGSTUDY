#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

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

UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::CreatePlayMontageAndWaitProxy(FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	if (UAbilityTask_PlayMontageAndWait* InWait= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TaskInstanceName, 
		InMontageToPlay, 
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
