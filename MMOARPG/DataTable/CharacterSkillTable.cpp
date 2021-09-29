#include "CharacterSkillTable.h"
#include "Abilities/GameplayAbility.h"

FCharacterSkillTable::FCharacterSkillTable()
	:ID(INDEX_NONE)
{

}

TSubclassOf<UGameplayAbility>* FCharacterSkillTable::FindComboAttack(const FName& InKey)
{
	return ComboAttack.FindByPredicate([&](TSubclassOf<UGameplayAbility> InGameplayAbility)->bool
		{
			return InGameplayAbility.GetDefaultObject()->AbilityTags
			== FGameplayTagContainer(FGameplayTag::RequestGameplayTag(InKey));
		});
}

TSubclassOf<UGameplayAbility>* FCharacterSkillTable::FindSkillAttack(const FName& InKey)
{
	return SkillAttack.FindByPredicate([&](TSubclassOf<UGameplayAbility> InGameplayAbility)->bool
		{
			return InGameplayAbility.GetDefaultObject()->AbilityTags
				== FGameplayTagContainer(FGameplayTag::RequestGameplayTag(InKey));
		});
}

