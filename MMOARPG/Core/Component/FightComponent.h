// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "../Game/Abilities/MMOARPGAbilitySystemComponent.h"
#include "SImpleComboType.h"
#include "FightComponent.generated.h"

class UMMOARPGGameplayAbility;
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

	UFUNCTION(BlueprintCallable)
		void NormalAttack(const FName& InKey);

	FSimpleComboCheck* GetSimpleComboInfo() { return &ComboAttack; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//������ӽӿ�
	FGameplayAbilitySpecHandle AddAbility(TSubclassOf<UGameplayAbility> InNewAbility);
	//���ܻ�ȡ�ӿ�
	UMMOARPGGameplayAbility* GetGamePlayAbility(const FName &InKey);

protected:
	void RegisterComboAttack(FSimpleComboCheck& InComboAttack, const FName& Key);

protected:
	//Map��Ч��ѯ
	TMap<FName, FGameplayAbilitySpecHandle> Skills;
};
