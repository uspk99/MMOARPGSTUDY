// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MMOARPGType.h"
#include "HallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	FCharacterAppearances& GetCharacterAppearances();

	bool IsCharacterExistInSlot(const int32 InPos);

	FMMOARPGCharacterAppearance* GetRecentCharacter();

	FMMOARPGCharacterAppearance* AddCharacterCA(const int32 InPos);
	FMMOARPGCharacterAppearance* GetCharacterCA(const int32 InPos);

	int32 AddCharacterCA(const FMMOARPGCharacterAppearance &InCA);

	//��ȡ��ʱ��ɫ
	FMMOARPGCharacterAppearance* GetCurrentTmpCreateCharacter() { return &CurrentTmpCreateCharacter;}

	//ɾ����ɫ
	int32 RemoveCharacterAppearanceBySlotID(const int32 InSlotID);
private:
	FCharacterAppearances CharacterAppearances;
	FMMOARPGCharacterAppearance CurrentTmpCreateCharacter;
	
};
