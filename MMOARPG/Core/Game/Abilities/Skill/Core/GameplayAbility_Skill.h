// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../MMOARPGGameplayAbility.h"
#include "GameplayAbility_Skill.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UGameplayAbility_Skill : public UMMOARPGGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void OnCompleted();

	virtual void OnBlendOut();

	virtual void OnInterrupted();

	virtual void OnCancelled();
	
};
