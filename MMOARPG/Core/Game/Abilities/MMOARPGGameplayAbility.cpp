#include "MMOARPGGameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

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

UAbilityTask_PlayMontageAndWait* UMMOARPGGameplayAbility::CreatePlayMontageAndWaitProxy(FName TaskInstanceName, UAnimMontage* InMontageToPlay, float Rate /*= 1.f*/, FName StartSection /*= NAME_None*/, bool bStopWhenAbilityEnds /*= true*/, float AnimRootMotionTranslationScale /*= 1.f*/, float StartTimeSeconds /*= 0.f*/)
{
	if (UAbilityTask_PlayMontageAndWait* InWait= UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		TaskInstanceName, 
		InMontageToPlay, 
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
