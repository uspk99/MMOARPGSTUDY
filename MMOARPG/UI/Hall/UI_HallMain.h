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
	virtual void NativeConstruct();//构造

	virtual void NativeDestruct();//析构

	//呼入出角色命名
	void PlayRenameIn();
	void PlayRenameOut();
	//创建结束还原角色选择面板
	void ResetCharacterCreatePannel(bool bSpawnNewCharacter);

	//直接显示最近的角色
	void SpawnRecentCharacter();
	//高亮默认的最近角色
	void HighlightDefaultSelection();

	//检查命名
	void CheckRename(FString &InCharacterName);
	//创建角色
	void CreateCharacter(const FMMOARPGCharacterAppearance &InCA);

	//设置
	void SetSlotPosition(const int32 InSlotPostion);

	//删除角色请求
	void DeleteCharacter(int32 InSlotID);
	
	//设置角色编辑面板
	void SetEditCharacter(const FMMOARPGCharacterAppearance* InCA);
	//删除刚刚的角色
	void DestroyCharacter();
	//重置
	void ResetCharacterAppearance(FMMOARPGCharacterAppearance* InCA);
	//编辑角色
	enum ECAType
	{
		CA_EDITOR,
		CA_CREATE,
	};
	void ResetEidtorType();
	void ResetCreateType();
	//编辑已经生成的角色
	void EditCharacter(int32 InSlotID);

	//链接到DS服务器
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
