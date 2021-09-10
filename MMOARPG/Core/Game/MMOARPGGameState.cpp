// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGGameState.h"


AMMOARPGGameState::AMMOARPGGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(
		TEXT("/Game/DataTable/CharacterAnimTable.CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;
}

FCharacterAnimTable* AMMOARPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	if (TArray<FCharacterAnimTable*>* AnimTables = GetCharacterAnimTables())
	{
		if (AnimTables->Num())
		{
			if (auto Anim = AnimTables->FindByPredicate([&](FCharacterAnimTable* InAnimTable)
				{
					return InAnimTable->ID == InAnimTableID;
				}))
			{
				return *Anim;
			}
		}
	}
	return NULL;
	//return *(GetCharacterAnimTables()->FindByPredicate([&](FCharacterAnimTable* InAnimTable)
	//	{
	//		return InAnimTable->ID == InAnimTableID;
	//	}));
}

TArray<FCharacterAnimTable*>* AMMOARPGGameState::GetCharacterAnimTables()
{
	if (!CharacterAnimTables.Num())
	{
		if (CharacterAnimTablePtr)
		{
			CharacterAnimTablePtr->GetAllRows(TEXT("AnimTable"), CharacterAnimTables);
		}
	}
	return &CharacterAnimTables;
}
