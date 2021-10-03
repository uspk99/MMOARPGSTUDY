#pragma once

#include "MMOARPGGameplayAbilityMacros.h"

#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"
#include "MMOARPGType.h"
#include "MMOARPGAttributeSet.generated.h"


UCLASS()
class MMOARPG_API UMMOARPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMMOARPGAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Health",ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	PROPERTY_FUNCTION_REGISTRATION(UMMOARPGAttributeSet, MaxMana);

	//代理通知  造成的效果（影响）
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	virtual void RegistrationProperties(const FMMOARPGCharacterAttribute& Data);

protected:
	void RegistrationParam(FGameplayAttributeData &AttributeData, const FMMOARPGAttributeData &InData);
protected:
	UFUNCTION()
		virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};