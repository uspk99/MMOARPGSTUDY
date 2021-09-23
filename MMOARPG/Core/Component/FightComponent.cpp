// Fill out your copyright notice in the Description page of Project Settings.


#include "FightComponent.h"
#include "MMOARPG/Core/Game/MMOARPGGameState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Game/Character/Core/MMOARPGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Abilities/GameplayAbility.h"
#include "../Game/Abilities/MMOARPGGameplayAbility.h"

UFightComponent::UFightComponent()
{

}

void UFightComponent::NormalAttack(const FName &InKey)
{
	//弱指针
	if (AbilitySystemComponent.IsValid())
	{
		if (FGameplayAbilitySpecHandle* Handle = Skills.Find(TEXT("NormalAttack")))
		{
			AbilitySystemComponent->TryActivateAbility(*Handle);
		}
	}
}

void UFightComponent::BeginPlay()
{
	Super::BeginPlay();
	if (MMOARPGCharacterBase.IsValid())
	{
		AbilitySystemComponent =Cast<UMMOARPGAbilitySystemComponent>(MMOARPGCharacterBase->GetAbilitySystemComponent());

		if (GetWorld())
		{
			if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>())
			{
				//添加固有技能
				if (FCharacterSkillTable* InSkillTable = InGameState->GetCharacterSkillTable(MMOARPGCharacterBase->GetID()))
				{
					Skills.Add(TEXT("NormalAttack"), AddAbility(InSkillTable->NormalAttack));
				}
			}
			//注册
			AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
		}
		RegisterComboAttack(ComboAttack, TEXT("NormalAttack"));

	}

}

FGameplayAbilitySpecHandle UFightComponent::AddAbility(TSubclassOf<UGameplayAbility> InNewAbility)
{
	if (IsValid(InNewAbility) && AbilitySystemComponent.IsValid())
	{
		return AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(InNewAbility));
	}
	return FGameplayAbilitySpecHandle();
}

UMMOARPGGameplayAbility* UFightComponent::GetGamePlayAbility(const FName& InKey)
{
	if (FGameplayAbilitySpecHandle *InHandle = Skills.Find(InKey))
	{
		if (AbilitySystemComponent.IsValid())
		{
			if (FGameplayAbilitySpec *Spec = AbilitySystemComponent->FindAbilitySpecFromHandle(*InHandle))
			{
				return Cast<UMMOARPGGameplayAbility>(Spec->Ability);
			}
			
		}
	}
	return nullptr;
}

void UFightComponent::RegisterComboAttack(FSimpleComboCheck& InComboAttack, const FName& Key)
{
	InComboAttack.Character = MMOARPGCharacterBase.Get();
	InComboAttack.ComboKey = Key;

	if (UMMOARPGGameplayAbility* GA = GetGamePlayAbility(Key))
	{
		InComboAttack.MaxIndex = GetGamePlayAbility(Key)->GetCompositeSectionsNumber();
	}
}
