#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterSkillTable.generated.h"


class UGameplayAbility;

USTRUCT(BlueprintType)
struct FCharacterSkillTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		FCharacterSkillTable();

		UPROPERTY(EditDefaultsOnly,Category="CharacterSkill")
		int32 ID;


		//UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		//	TMap<FName, TSubclassOf<UGameplayAbility>> ComboAttack;

		//UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
		//	TMap<FName, TSubclassOf<UGameplayAbility>> SkillAttack;

		UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
			TArray<TSubclassOf<UGameplayAbility>> ComboAttack;

		UPROPERTY(EditDefaultsOnly, Category = "CharacterSkill")
			TArray<TSubclassOf<UGameplayAbility>> SkillAttack;

		TSubclassOf<UGameplayAbility>* FindComboAttack(const FName &InKey);
		TSubclassOf<UGameplayAbility>* FindSkillAttack(const FName& InKey);
};
