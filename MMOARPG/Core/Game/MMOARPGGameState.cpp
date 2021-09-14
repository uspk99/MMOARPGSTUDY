// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGGameState.h"


AMMOARPGGameState::AMMOARPGGameState()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterAnimTable(
		TEXT("/Game/DataTable/CharacterAnimTable"));
	CharacterAnimTablePtr = CharacterAnimTable.Object;
	static ConstructorHelpers::FObjectFinder<UDataTable> CharacterStyleTable(
		TEXT("/Game/DataTable/CharacterStyleTable"));
	CharacterStyleTablePtr = CharacterStyleTable.Object;
}

FCharacterAnimTable* AMMOARPGGameState::GetCharacterAnimTable(int32 InAnimTableID)
{
	return GetTable<FCharacterAnimTable>(
		InAnimTableID,
		CharacterAnimTablePtr,
		CharacterAnimTables,
		TEXT("AnimTable"));
}

TArray<FCharacterAnimTable*>* AMMOARPGGameState::GetCharacterAnimTables()
{
	return GetTables<FCharacterAnimTable>(
		CharacterAnimTablePtr,
		CharacterAnimTables,
		TEXT("AnimTable"));
}

FCharacterStyleTable* AMMOARPGGameState::GetCharacterStyleTable(int32 InStyleTableID)
{
	return GetTable<FCharacterStyleTable>(
		InStyleTableID,
		CharacterStyleTablePtr,
		CharacterStyleTables,
		TEXT("CharacterTable"));
}

TArray<FCharacterStyleTable*>* AMMOARPGGameState::GetCharacterStyleTables()
{
	return GetTables<FCharacterStyleTable>(
		CharacterStyleTablePtr,
		CharacterStyleTables,
		TEXT("CharacterTable"));
}
