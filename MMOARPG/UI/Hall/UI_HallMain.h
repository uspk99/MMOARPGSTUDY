// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "../Core/UI_MainBase.h"
#include "UI_HallMain.generated.h"


class UUI_CharacterCreatePannel;
class UUI_RenameCreate;
class UUI_EditorCharacter;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_HallMain : public UUI_MainBase
{
	GENERATED_BODY()



	UPROPERTY(meta = (BindWidget))
		UUI_CharacterCreatePannel* UI_CharacterCreatePannel;

	UPROPERTY(meta = (BindWidget))
		UUI_RenameCreate* UI_RenameCreate;

	UPROPERTY(meta = (BindWidget))
	UUI_EditorCharacter* UI_EditorCharacter;
public:
	virtual void NativeConstruct();//����

	virtual void NativeDestruct();//����

	//�������ɫ����
	void PlayRenameIn();
	void PlayRenameOut();
	//����������ԭ��ɫѡ�����
	void ResetCharacterCreatePannel(bool bSpawnNewCharacter);

	//ֱ����ʾ����Ľ�ɫ
	void SpawnRecentCharacter();
	//����Ĭ�ϵ������ɫ
	void HighlightDefaultSelection();

	//�������
	void CheckRename(FString &InCharacterName);
	//������ɫ
	void CreateCharacter(const FMMOARPGCharacterAppearance &InCA);

	//����
	void SetSlotPosition(const int32 InSlotPostion);

	//ɾ����ɫ����
	void DeleteCharacter(int32 InSlotID);
	
	//���ý�ɫ�༭���
	void SetEditCharacter(const FMMOARPGCharacterAppearance* InCA);
	//ɾ���ոյĽ�ɫ
	void DestroyCharacter();
	//����
	void ResetCharacterAppearance(FMMOARPGCharacterAppearance* InCA);
	//�༭��ɫ
	enum ECAType
	{
		CA_EDITOR,
		CA_CREATE,
	};
	void ResetEidtorType();
	void ResetCreateType();
	//�༭�Ѿ����ɵĽ�ɫ
	void EditCharacter(int32 InSlotID);

	//���ӵ�DS������
	void JumpDSServer(int32 InSlotID);

protected:

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);


		virtual void LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg);

	void PrintLogByCheckName(ECheckNameType InCheckNameType);

protected:
	void HallMainIn();
	void HallMainOut();
private:
	ECAType CAType = ECAType::CA_CREATE;
	FMMOARPGCharacterAppearance StartCAData;
};
