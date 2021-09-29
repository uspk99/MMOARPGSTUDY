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

void UFightComponent::Press_Implementation()
{
	ComboAttack.Press();
}

void UFightComponent::Released_Implementation()
{
	ComboAttack.Released();
}

void UFightComponent::Reset_Implementation()
{
	ComboAttack.Reset();
}

void UFightComponent::DodgeSkill_Implementation()
{
	NormalAttack(TEXT("Player.Skill.Dodge"));
}

void UFightComponent::SprintSkill_Implementation()
{
	NormalAttack(TEXT("Player.Skill.Sprint"));
}

void UFightComponent::NormalAttack(const FName &InKey)
{
	//弱指针
	if (AbilitySystemComponent.IsValid())
	{
		if (FGameplayAbilitySpecHandle* Handle = Skills.Find(InKey))
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

		//const FName InKey = TEXT("NormalAttack");
		const FName InKey = TEXT("Player.Attack.ComboLinkage");
		//服务器
		if (MMOARPGCharacterBase->GetLocalRole()==ENetRole::ROLE_Authority)
		{
			
			if (GetWorld())
			{
				AddMMOARPGGameplayAbility(InKey, EGameplayAbilityType::GAMEPLAYABILITY_COMBOATTACK);
				AddMMOARPGGameplayAbility(TEXT("Player.Skill.Dodge"), EGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
				AddMMOARPGGameplayAbility(TEXT("Player.Skill.Sprint"), EGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
				//要在服务器注册
				AbilitySystemComponent->InitAbilityActorInfo(MMOARPGCharacterBase.Get(), MMOARPGCharacterBase.Get());
			}	
		}
		//本地注册combo
		RegisterComboAttack(ComboAttack, InKey);
	}

}

void UFightComponent::AddMMOARPGGameplayAbility(const FName& InKey, EGameplayAbilityType InType)
{
	if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>())
	{
		//添加固有连击
		if (FCharacterSkillTable* InSkillTable = InGameState->GetCharacterSkillTable(MMOARPGCharacterBase->GetID()))
		{
			auto GetMMOARPGGameplayAbility = [&](EGameplayAbilityType InType)->TSubclassOf<UGameplayAbility>*
			{
				switch (InType)
				{
				case GAMEPLAYABILITY_SKILLATTACK:
					return InSkillTable->FindSkillAttack(InKey);
					break;
				case GAMEPLAYABILITY_COMBOATTACK:
					return InSkillTable->FindComboAttack(InKey);
					break;
				}
				return NULL;
			};
			if (TSubclassOf<UGameplayAbility>* InAbility = GetMMOARPGGameplayAbility(InType))
			{
				Skills.Add(InKey, AddAbility(*InAbility));
			}

			//if (TMap<FName, TSubclassOf<UGameplayAbility>> *NewGameplayAbilitys = GetMMOARPGGameplayAbility(InType))
			//{
			//	if (TSubclassOf<UGameplayAbility>* InAbility = NewGameplayAbilitys->Find(InKey))
			//	{
			//		Skills.Add(InKey, AddAbility(*InAbility));
			//	}
			//}
		}
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
