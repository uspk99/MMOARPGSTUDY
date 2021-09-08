// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_RenameCreate.generated.h"

class UEditableTextBox;
class UButton;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_RenameCreate : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
		UEditableTextBox* EditableName;
	UPROPERTY(meta = (BindWidget))
		UButton* FindNameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CreateButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CancelButton;

public:
	virtual void NativeConstruct();//¹¹Ôì

	virtual void NativeDestruct();//Îö¹¹

	void SetSlotPosition(int32 InSlotPosition);

	int32 GetSlotPosition() { return SlotPosition; }

	void SetEditableName(const FText &InName);
protected:
	UFUNCTION()
		void ClickedFindName();

	UFUNCTION()
		void ClickedCreate();

	UFUNCTION()
		void ClickedCancel();

protected:
	int32 SlotPosition;
};
