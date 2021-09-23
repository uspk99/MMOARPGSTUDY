// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_NormalAttack.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UGameplayAbility_NormalAttack::OnCompleted()
{
	Super::OnCompleted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnBlendOut()
{
	Super::OnBlendOut();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnInterrupted()
{
	Super::OnInterrupted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::OnCancelled()
{
	Super::OnCancelled();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_NormalAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Ã·Ωª
	if (!CommitAbility(Handle,ActorInfo,ActivationInfo))
	{
		return;
	}
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor))
	{
		if (PlayMontage(*FString::FromInt(InCharacterBase->GetSimpleComboInfo()->ComboIndex)))
		{

		}
	}

}
