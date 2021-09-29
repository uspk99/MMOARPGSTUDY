// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SImpleComboType.h"
#include "FightComponent.generated.h"

class UMMOARPGGameplayAbility;
enum EGameplayAbilityType
{
	GAMEPLAYABILITY_SKILLATTACK,
	GAMEPLAYABILITY_COMBOATTACK,

};
/**
 * 
 */
UCLASS()
class MMOARPG_API UFightComponent : public UMotionComponent
{
	GENERATED_BODY()

		UPROPERTY()
		TWeakObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	FSimpleComboCheck ComboAttack;
public:
	UFightComponent();

	UFUNCTION(NetMulticast, Reliable)
	void Press();
	UFUNCTION(NetMulticast, Reliable)
	void Released();
	UFUNCTION(NetMulticast, Reliable)
	void Reset();

	UFUNCTION(NetMulticast, Reliable)
	void DodgeSkill();

	UFUNCTION(NetMulticast, Reliable)
		void SprintSkill();

	UFUNCTION(BlueprintCallable)
		void NormalAttack(const FName& InKey);

	FSimpleComboCheck* GetSimpleComboInfo() { return &ComboAttack; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void AddMMOARPGGameplayAbility(const FName& InKey,EGameplayAbilityType InType = EGameplayAbilityType::GAMEPLAYABILITY_SKILLATTACK);
	//技能添加接口
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);
	//技能获取接口
	UMMOARPGGameplayAbility* GetGamePlayAbility(const FName &InKey);

protected:
	void RegisterComboAttack(FSimpleComboCheck& InComboAttack, const FName& Key);

protected:
	//Map高效查询
	TMap<FName, FGameplayAbilitySpecHandle> Skills;
};
