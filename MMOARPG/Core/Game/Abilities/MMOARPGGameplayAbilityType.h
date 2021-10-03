#pragma once

#include "CoreMinimal.h"
#include "MMOARPGGameplayAbilityType.generated.h"

USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffects
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=GameplayEffects)
	TArray<TSubclassOf<UGameplayEffect>> TargetEffectClasses;
};

USTRUCT(BlueprintType)
struct FMMOARPGGameplayEffectSpec
{
	GENERATED_USTRUCT_BODY()

public:
	//目标的数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		FGameplayAbilityTargetDataHandle TargetHandleData;
	//效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = GameplayEffects)
		TArray<FGameplayEffectSpecHandle> TargetEffectSpecs;
};
