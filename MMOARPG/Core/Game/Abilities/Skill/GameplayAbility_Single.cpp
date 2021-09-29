// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility_Single.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

void UGameplayAbility_Single::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//Ã·Ωª
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}
	if (AMMOARPGCharacterBase* InCharacterBase = Cast<AMMOARPGCharacterBase>(ActorInfo->OwnerActor))
	{
		if (PlayMontage(*FString::FromInt(0)))
		{

		}
	}
}
