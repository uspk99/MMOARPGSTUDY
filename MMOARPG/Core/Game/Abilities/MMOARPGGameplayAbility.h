#pragma once

#include "Abilities/GameplayAbility.h"
#include "AbilityTask/AbilityTask_PMAWDamageEvent.h"
#include "MMOARPGGameplayAbilityType.h"
#include "MMOARPGGameplayAbility.generated.h"

class UAbilityTask_PlayMontageAndWait;
UCLASS()
class MMOARPG_API UMMOARPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMMOARPGGameplayAbility();
	//子类去具体实现
	UPROPERTY(EditDefaultsOnly,Category= MontageAbility)
	UAnimMontage* MontageToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
	TMap<FGameplayTag,FMMOARPGGameplayEffects> EffectMap;

public:
	int32 GetCompositeSectionsNumber();

	//C++
	UFUNCTION()
	 virtual void OnCompleted();

	UFUNCTION()
	virtual void OnBlendOut();

	UFUNCTION()
	virtual void OnInterrupted();

	UFUNCTION()
	virtual void OnCancelled();

	//自定义绑定接收函数,伤害
	UFUNCTION()
	virtual	void OnDamageGameplayEvent(FGameplayTag InGameplayTag, const FGameplayEventData Payload);

public:
	//蓝图写的
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnCancelled", meta = (ScriptName = "OnCancelled"))
		void K2_OnCancelled();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnInterrupted", meta = (ScriptName = "OnInterrupted"))
		void K2_OnInterrupted();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnBlendOut", meta = (ScriptName = "OnBlendOut"))
		void K2_OnBlendOut();
	UFUNCTION(BlueprintImplementableEvent, Category = Ability, DisplayName = "OnCompleted", meta = (ScriptName = "OnCompleted"))
		void K2_OnCompleted();

public:
	UFUNCTION(BlueprintCallable, 
		Category = "MMOARPGGameplayAbility|Tasks", 
		meta = (DisplayName = "PlayMontageAndWait",
		HidePin = "OwningAbility", 
			DefaultToSelf = "OwningAbility", 
			BlueprintInternalUseOnly = "TRUE"))
	UAbilityTask_PlayMontageAndWait* CreatePlayMontageAndWaitProxy(
		FName TaskInstanceName, 
		UAnimMontage* InMontageToPlay,
		float Rate = 1.f, 
		FName StartSection = NAME_None, 
		bool bStopWhenAbilityEnds = true, 
		float AnimRootMotionTranslationScale = 1.f, 
		float StartTimeSeconds = 0.f);

	UFUNCTION(BlueprintCallable,Category = "MMOARPGGameplayAbility|Tasks")
	UAbilityTask_PlayMontageAndWait* PlayMontage(FName StartSection = NAME_None);
};