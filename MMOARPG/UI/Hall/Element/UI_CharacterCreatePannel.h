// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "MMOARPGType.h"
#include "Components/ScrollBox.h"
#include "UI_CharacterCreatePannel.generated.h"

class UTextBlock;
class UScrollBox;
class UUI_CharacterButton;
class UUI_KneadFace;
class ACharacterStage;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterCreatePannel : public UUI_Base
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlaneName;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* List;
	
	//��¶����ͼ
	UPROPERTY(EditDefaultsOnly , Category = UI)
	TSubclassOf<UUI_CharacterButton> UI_CharacterButtonClass;

	//��¶����ͼ
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_KneadFace> UI_KneadFaceClass;

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

	UUI_KneadFace* CreateKneadFace();
	void CreateCharacterButton();
	void InitCharacterButtons(FCharacterAppearances& InCAs);

	ACharacterStage* CreateCharacter();
	void SpawnCharacter(const int32 InSlotPos);
	void SpawnCharacter(const FMMOARPGCharacterAppearance *InACData);
	void SpawnCharacter();
	
	void SetCurrentSlotPositon(const int32 InNewPos);

	//bool����ǰ�߼��Ƿ����ѭ��
	template<class T>
	T* FindByPerdicateInList(TFunction<bool(T*)> Func)
	{
		for (auto& Tmp : List->GetAllChildren())
		{
			if (T* InButton = Cast<T>(Tmp))
			{
				if (Func(InButton))
				{
					return InButton;
					break;
				}
			}
		}
		return NULL;
	}
	//����������λ
	void HighlightDefaultSelection(const int32 InNewPos);

	//��ȡ��������
	UUI_CharacterButton* GetHighlightButton();

	void JumpDSServer(int32 InSlotID);
protected:
	
	int32 SlotPositon;//��ǰѡ���ɫ���

	//void InitCharacterButton(const int32 InNumber);
	

};
