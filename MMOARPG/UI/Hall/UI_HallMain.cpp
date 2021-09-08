// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_HallMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "Protocol/LoginProtocol.h"
#include "../../MMOARPGMacroType.h"
#include "../Common/UI_Print.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Element/UI_CharacterCreatePannel.h"
#include "Element/UI_RenameCreate.h"
#include "Protocol/HallProtocol.h"
#include "../../Core/Hall/HallPlayerState.h"
#include "MMOARPGType.h"
#include "Element/KneadFace/UI_EditorCharacter.h"
#include "../../Core/Hall/HallPawn.h"
#include "D:\UE5study\MMOARPG\Source\MMOARPG\Core\Hall\Character\CharacterStage.h"
#include "Element/UI_KneadFace.h"

#define LOCTEXT_NAMESPACE "UUI_HallMain"
void UUI_HallMain::NativeConstruct()
{
	Super::NativeConstruct();
	HallMainIn();

	UI_CharacterCreatePannel->SetParents(this);
	UI_RenameCreate->SetParents(this);
	UI_EditorCharacter->SetParents(this);
	
	CAType = ECAType::CA_CREATE;

	UI_RenameCreate->SetVisibility(ESlateVisibility::Collapsed);

	LinkServer();
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		LinkServer(InGameInstance->GetGateStatus().GateServerAddrInfo.Addr);
	}
}

void UUI_HallMain::NativeDestruct()
{
	Super::NativeDestruct();

}


void UUI_HallMain::PlayRenameIn()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameIn"));
}

void UUI_HallMain::PlayRenameOut()
{
	UI_RenameCreate->PlayWidgetAnim(TEXT("RenameOut"));
}

void UUI_HallMain::ResetCharacterCreatePannel(bool bSpawnNewCharacter)
{
	UI_CharacterCreatePannel->CreateCharacterButton();
	bool bInit = false;
	bInit = bSpawnNewCharacter;
	//点击取消后重新生成最近对象
	if (bInit)
	{
		SpawnRecentCharacter();
	}

	HighlightDefaultSelection();
}

void UUI_HallMain::SpawnRecentCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMOARPGCharacterAppearance* InCAData = InState->GetRecentCharacter())
		{
			UI_CharacterCreatePannel->SpawnCharacter(InCAData);
			SetEditCharacter(InCAData);
		}
		else
		{
			SetEditCharacter(NULL);
			DestroyCharacter();
		}
	}
}

void UUI_HallMain::HighlightDefaultSelection()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMOARPGCharacterAppearance* InCAData = InState->GetRecentCharacter())
		{
			UI_CharacterCreatePannel->HighlightDefaultSelection(InCAData->SlotPosition);
		}
	}
}

void UUI_HallMain::CheckRename(FString& InCharacterName)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		SEND_DATA(SP_CheckCharacterNameRequests, InGameInstance->GetUserData().ID, InCharacterName);
	}
}

void UUI_HallMain::CreateCharacter(const FMMOARPGCharacterAppearance& InCA)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		FString CAJson;
		if (CAType==ECAType::CA_EDITOR)
		{
			if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
			{
				if (FMMOARPGCharacterAppearance* InNewCA = InState->GetCharacterCA(UI_RenameCreate->GetSlotPosition()))
				{
					InNewCA->Date = InCA.Date;
					InNewCA->Name = InCA.Name;

					NetDataAnalysis::CharacterAppearancesToString(*InNewCA, CAJson);
				}
			}
			SEND_DATA(SP_EditorCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}
		else
		{
			NetDataAnalysis::CharacterAppearancesToString(InCA, CAJson);
			SEND_DATA(SP_CreateCharacterRequests, InGameInstance->GetUserData().ID, CAJson);
		}	
	}

}

void UUI_HallMain::SetSlotPosition(const int32 InSlotPostion)
{
	UI_RenameCreate->SetSlotPosition(InSlotPostion);
}

void UUI_HallMain::DeleteCharacter(int32 InSlotID)
{
	if (InSlotID>=0&&InSlotID<4)
	{
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
		{
			//发送角色形象请求
			SEND_DATA(SP_DeleteCharacterRequests, InGameInstance->GetUserData().ID, InSlotID);
		}
	}
}

void UUI_HallMain::SetEditCharacter(const FMMOARPGCharacterAppearance* InCA)
{
	if (InCA)
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(InCA->Name));
		UI_EditorCharacter->SetSoltID(InCA->SlotPosition);
	}
	else
	{
		UI_EditorCharacter->SetCharacterName(FText::FromString(""));
		UI_EditorCharacter->SetSoltID(INDEX_NONE);
	}
}

void UUI_HallMain::DestroyCharacter()
{
	//删除刚刚角色
	if (AHallPawn* InPawn =GetPawn<AHallPawn>())
	{
		if (InPawn->CharacterStage)
		{
			InPawn->CharacterStage->Destroy();
			InPawn->CharacterStage = nullptr;
		}
		
	}
}

void UUI_HallMain::ResetCharacterAppearance(FMMOARPGCharacterAppearance* InCA)
{
	if (CAType==ECAType::CA_EDITOR)
	{
		if (InCA)
		{
			*InCA = StartCAData;
		}
	}
	ResetCreateType();
	UI_EditorCharacter->SetIsEnabled(true);
}

void UUI_HallMain::ResetEidtorType()
{
	CAType = ECAType::CA_EDITOR;
}

void UUI_HallMain::ResetCreateType()
{
	CAType = ECAType::CA_CREATE;
}

void UUI_HallMain::EditCharacter(int32 InSlotID)
{
	PlayRenameIn();
	SetSlotPosition(InSlotID);

	if (UUI_KneadFace* InKneadFace = UI_CharacterCreatePannel->CreateKneadFace())
	{
		if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
		{
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(InSlotID))
			{
				InKneadFace->InitKneadFace(InCA);
				StartCAData = *InCA;

				ResetEidtorType();
				UI_EditorCharacter->SetIsEnabled(false);
				//设置名称
				UI_RenameCreate->SetEditableName(FText::FromString(InCA->Name));
			}
		}
	}

}

void UUI_HallMain::JumpDSServer(int32 InSlotID)
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		SEND_DATA(SP_LoginToDSServerRequests,InGameInstance->GetUserData().ID, InSlotID);
	}
}


void UUI_HallMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case SP_CharacterAppearanceResponses:
	{
		FString CharacterJson;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);
		if (!CharacterJson.IsEmpty())
		{
			if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
			{
				//FCharacterAppearances test;
				//NetDataAnalysis::StringToCharacterAppearances(CharacterJson, test);
				//FMMOARPGCharacterAppearance ttt;
				//NetDataAnalysis::StringToCharacterAppearances(CharacterJson, ttt);

				NetDataAnalysis::StringToCharacterAppearances(CharacterJson, InState->GetCharacterAppearances());
				UI_CharacterCreatePannel->InitCharacterButtons(InState->GetCharacterAppearances());

				//生成最近角色
				SpawnRecentCharacter();

				//高亮
				HighlightDefaultSelection();
			}			
		}
		break;
	}
	case SP_CheckCharacterNameResponses:
	{
		ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CheckCharacterNameResponses,CheckNameType);
		switch (CheckNameType)
		{
		case UNKNOWN_ERROR:
			PrintLog(LOCTEXT("CHECK_NAME_UNKNOWN_ERROR", "The server encountered an unknown error."));
			break;
		case NAME_NOT_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is valid"));
			break;
		case SERVER_NOT_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
			break;
		case NAME_EXIST:
			PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
			break;
		default:
			break;
		}
		break;
	}
	case SP_CreateCharacterResponses:
	{
		ECheckNameType CheckNameType = ECheckNameType::UNKNOWN_ERROR;
		bool bCreateCharacter = false;
		FString JsonString;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, CheckNameType, bCreateCharacter, JsonString);
		if (bCreateCharacter)
		{
			PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_SUCCESSFULLY", "created successfully."));

			FMMOARPGCharacterAppearance InCA;
			NetDataAnalysis::StringToCharacterAppearances(JsonString, InCA);

			if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
			{
				InState->AddCharacterCA(InCA);
				//成功创建后再退出
				PlayRenameOut();
				//重新创建面板
				ResetCharacterCreatePannel(false);

				//重置编辑面板
				SetEditCharacter(&InCA);
			}
		}
		else
		{
			PrintLog(LOCTEXT("CREATECHARACTERRESPONSES_FAIL", "created fail."));
			//= 传值
			GThread::Get()->GetCoroutines().BindLambda(1.5f, [=]() { PrintLogByCheckName(CheckNameType); });
		}
		break;
	}
	case SP_DeleteCharacterResponses:
	{
		int32 UserID;
		int32 SlotID;
		int32 SuccessDeleteCount;
		SIMPLE_PROTOCOLS_RECEIVE(SP_CreateCharacterResponses, UserID, SlotID, SuccessDeleteCount);
		if (SuccessDeleteCount>2)
		{
			if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
			{
				InState->RemoveCharacterAppearanceBySlotID(SlotID);
				ResetCharacterCreatePannel(true);
			}
			PrintLog(LOCTEXT("DELETE_CHARACTER_SUCCESS","The role deletion is successful."));
		}
		else
		{
			PrintLog(LOCTEXT("DELETE_CHARACTER_ERROR", "Failed to delete the role."));
		}

		break;
	}
	case SP_EditorCharacterResponses:
	{
		FSimpleAddrInfo AddrInfo;
		bool bUpdateSucceeded = false;

		SIMPLE_PROTOCOLS_RECEIVE(SP_EditorCharacterResponses, bUpdateSucceeded);
		if (bUpdateSucceeded)
		{
			
			PrintLog(LOCTEXT("EDITORCHARACTERRESPONSE_SSUCCESS","EditorCharacter Success"));
		}
		else
		{
			PrintLog(LOCTEXT("EDITORCHARACTERRESPONSE_ERROR", "EditorCharacter Failed"));
		}
		//淡出
		PlayRenameOut();
		ResetCharacterAppearance(&StartCAData);
		ResetCharacterCreatePannel(false);
		break;
	}
	case SP_LoginToDSServerResponses:
	{
		FSimpleAddr DSServerAddr;
		SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses,DSServerAddr);

		FString DSAddrString = FSimpleNetManage::GetAddrString(DSServerAddr);
		
		HallMainOut();
		
		GThread::Get()->GetCoroutines().BindLambda(1.f,
			[=]()
			{
				UGameplayStatics::OpenLevel(GetWorld(), *DSAddrString);
			});
	}
	default:
		break;
	}
}

void UUI_HallMain::LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg)
{
	if (Intype == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
		{
			//发送角色形象请求
			SEND_DATA(SP_CharacterAppearanceRequests, InGameInstance->GetUserData().ID)
		}
	}
}

void UUI_HallMain::PrintLogByCheckName(ECheckNameType InCheckNameType)
{
	switch (InCheckNameType)
	{
	case UNKNOWN_ERROR:
		PrintLog(LOCTEXT("CHECK_NAME_UNKNOWN_ERROR", "The server encountered an unknown error."));
		break;
	case NAME_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_NOT_EXIST", "The name is valid"));
		break;
	case SERVER_NOT_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_SERVER_NOT_EXIST", "Server error."));
		break;
	case NAME_EXIST:
		PrintLog(LOCTEXT("CHECK_NAME_NAME_EXIST", "The name has been registered."));
		break;
	}
}

void UUI_HallMain::HallMainIn()
{
	PlayWidgetAnim(TEXT("HallMainIn"));
}

void UUI_HallMain::HallMainOut()
{
	PlayWidgetAnim(TEXT("HallMainOut"));
}

#undef LOCTEXT_NAMESPACE