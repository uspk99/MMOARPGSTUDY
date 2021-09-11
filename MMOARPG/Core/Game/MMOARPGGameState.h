// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "MMOARPGGameState.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
		AMMOARPGGameState();
		//获取表中内容
		FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);
		//获取表
		TArray<FCharacterAnimTable*>* GetCharacterAnimTables();
protected:
	//表指针
	UPROPERTY()
		UDataTable* CharacterAnimTablePtr;
	//缓存
	TArray<FCharacterAnimTable*> CharacterAnimTables;
};
