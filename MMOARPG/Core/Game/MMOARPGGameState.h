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
		//��ȡ��������
		FCharacterAnimTable* GetCharacterAnimTable(int32 InAnimTableID);
		//��ȡ��
		TArray<FCharacterAnimTable*>* GetCharacterAnimTables();
protected:
	//��ָ��
	UPROPERTY()
		UDataTable* CharacterAnimTablePtr;
	//����
	TArray<FCharacterAnimTable*> CharacterAnimTables;
};
