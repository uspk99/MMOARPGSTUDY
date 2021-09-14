// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "../../DataTable/CharacterAnimTable.h"
#include "../../DataTable/CharacterStyleTable.h"
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

		//��ȡ��������
		FCharacterStyleTable* GetCharacterStyleTable(int32 InStyleTableID);
		//��ȡ��
		TArray<FCharacterStyleTable*>* GetCharacterStyleTables();
protected:
		//ģ��
		template<class T>
		TArray<T*>* GetTables(
			UDataTable* InTable,
			TArray<T*> &OutTables,
			const FString &MsgTag=TEXT("MsgTable"))
		{
			if (!OutTables.Num())
			{
				if (InTable)
				{
					InTable->GetAllRows(MsgTag, OutTables);
				}
			}
			return &OutTables;
		}

		template<class T>
		T* GetTable(int32 InTableID,
			UDataTable* InTable,
			TArray<T*>& OutTables,
			const FString& MsgTag = TEXT("MsgTable"))
		{
			if (TArray<T*>* Tables = GetTables(InTable, OutTables, MsgTag))
			{
				if (Tables->Num())
				{
					if (auto TheTable = Tables->FindByPredicate([&](T* InMyTable)
						{
							return InMyTable->ID == InTableID;
						}))
					{
						return *TheTable;
					}
				}
			}
			return NULL;
		}

protected:
	//��ָ��
	UPROPERTY()
		UDataTable* CharacterAnimTablePtr;

	UPROPERTY()
		UDataTable* CharacterStyleTablePtr;
	//����
	TArray<FCharacterAnimTable*> CharacterAnimTables;

	TArray<FCharacterStyleTable*> CharacterStyleTables;
};
