// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMOARPGType.h"
#include "UI_CharacterButton.generated.h"

class UImage;
class UVerticalBox;
class UButton;
class UTextBlock;
class ACharacterStage;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterButton : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* CreateShowIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	UButton* CharacterButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Date;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		FLinearColor HighlightColor;
	//��ɫ����
#if 1
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<ACharacterStage> CharacterStageClass;

	UPROPERTY(EditDefaultsOnly, Category = UI)
		FVector SpawnPoint;
#endif

public:
	virtual void NativeConstruct();//����

	virtual void NativeDestruct();//����

	void SetSlotPosition(const int32 InNewPos);

	void InitCharacterButton(const FMMOARPGCharacterAppearance& InCA);

	//������ʾ
	void SetHighLight(bool bHighLight);

	//�жϸ���
	bool IsHighLight();

	//��ȡ��ɫ�۵�λ��
	FORCEINLINE int32 GetSlotPositon() { return SlotPositon; }

	void JumpDSServer();
protected:
	UFUNCTION()
	void ClickedCharacter();

	int32 SlotPositon;
	FLinearColor DefaultColor;
};
