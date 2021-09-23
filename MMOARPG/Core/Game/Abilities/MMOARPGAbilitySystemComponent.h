// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "MMOARPGAbilitySystemComponent.generated.h"

UCLASS()
class MMOARPG_API UMMOARPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UMMOARPGAbilitySystemComponent();

	static UMMOARPGAbilitySystemComponent* GetAbilitySystemComponentFromActor(
		const AActor* Actor, bool LookForComponent = false);
};