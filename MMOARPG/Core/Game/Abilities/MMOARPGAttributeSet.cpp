#include "MMOARPGAttributeSet.h"

UMMOARPGAttributeSet::UMMOARPGAttributeSet()
	:Health(100.f)
	,MaxHealth(100.f)
	,Mana(0.f)
	,MaxMana(0.f)
{
	
}

void UMMOARPGAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UMMOARPGAttributeSet::RegistrationProperties(const FMMOARPGCharacterAttribute& Data)
{
	RegistrationParam(Health,Data.Health);
	RegistrationParam(MaxHealth, Data.MaxHealth);
	RegistrationParam(Mana, Data.Mana);
	RegistrationParam(MaxMana, Data.MaxMana);
}

void UMMOARPGAttributeSet::RegistrationParam(FGameplayAttributeData& AttributeData, const FMMOARPGAttributeData& InData)
{
	AttributeData.SetBaseValue(InData.BaseValue);
	AttributeData.SetCurrentValue(InData.CurrentValue);
}

void UMMOARPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
//获取变量，设置属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet,Health,OldValue);
}

void UMMOARPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MaxHealth, OldValue);
}


void UMMOARPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, Mana, OldValue);
}


void UMMOARPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMMOARPGAttributeSet, MaxMana, OldValue);
}

void UMMOARPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMMOARPGAttributeSet, Health);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxHealth);
	DOREPLIFETIME(UMMOARPGAttributeSet, Mana);
	DOREPLIFETIME(UMMOARPGAttributeSet, MaxMana);
}

