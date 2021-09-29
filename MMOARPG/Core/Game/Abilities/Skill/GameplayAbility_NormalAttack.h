// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameplayAbility_Skill.h"
#include "GameplayAbility_NormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UGameplayAbility_NormalAttack : public UGameplayAbility_Skill
{
	GENERATED_BODY()
public:

	virtual void OnCompleted();

	virtual void OnBlendOut();

	virtual void OnInterrupted();

	virtual void OnCancelled();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData);
};
