// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include  "../../../Core/UI_KneadFaceBase.h"
#include "UI_EditorCharacter.generated.h"

class UButton;
class UEditableTextBox;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_EditorCharacter : public UUI_KneadFaceBase
{
	GENERATED_BODY()

		UPROPERTY(meta=(BindWidget))
		UButton* EditorCharacterButton;
	UPROPERTY(meta = (BindWidget))
		UButton* DeleteCharacterButton;
	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* CharacterName;

public:
	virtual void NativeConstruct();//¹¹Ôì

	virtual void NativeDestruct();//Îö¹¹

	void SetCharacterName(const FText& InName);

	void SetSoltID(int32 NewSlotID) { SlotID = NewSlotID; }

private:
	UFUNCTION()
	void EditCharacter();
	UFUNCTION()
	void DeleteCharacter();
private:
	int32 SlotID;
};
