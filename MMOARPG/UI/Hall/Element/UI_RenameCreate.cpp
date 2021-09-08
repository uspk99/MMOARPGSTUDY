// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_RenameCreate.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "../UI_HallMain.h"
#include "MMOARPG/Core/Hall/HallPlayerState.h"
#include "MMOARPG/Core/Hall/HallPawn.h"
#include "MMOARPG/Core/Hall/Character/CharacterStage.h"

void UUI_RenameCreate::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPosition = INDEX_NONE;

	FindNameButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedFindName);
	CreateButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCreate);
	CancelButton->OnReleased.AddDynamic(this, &UUI_RenameCreate::ClickedCancel);
}

void UUI_RenameCreate::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_RenameCreate::SetSlotPosition(int32 InSlotPosition)
{
	SlotPosition = InSlotPosition;
}

void UUI_RenameCreate::SetEditableName(const FText &InName)
{
	EditableName->SetText(InName);
}

void UUI_RenameCreate::ClickedFindName()
{
	if (UUI_HallMain* UI_HallMain = GetParents<UUI_HallMain>())
	{
		FString CharcterName = EditableName->GetText().ToString();
		UI_HallMain->CheckRename(CharcterName);
	}
}

void UUI_RenameCreate::ClickedCreate()
{
	if (UUI_HallMain* UI_HallMain = GetParents<UUI_HallMain>())
	{
		if (AHallPlayerState* InPlayerState = GetPlayerState<AHallPlayerState>())
		{
			//if (FMMOARPGCharacterAppearance* InCA = InPlayerState->AddCharacterCA(SlotPosition))
			{
				if (auto* CurrentTmpCreateCharacter = InPlayerState->GetCurrentTmpCreateCharacter())
				{
					CurrentTmpCreateCharacter->Name = EditableName->GetText().ToString();
					CurrentTmpCreateCharacter->Date = FDateTime::Now().ToString();

					if (CurrentTmpCreateCharacter->Name.IsEmpty())
					{
						UI_HallMain->PrintLog(TEXT("Name Cannot be empty"));
					}
					else
					{
						UI_HallMain->CreateCharacter(*CurrentTmpCreateCharacter);
					}
				}
				//不创建临时数据
#if 0
				FMMOARPGCharacterAppearance InCA;
				InCA.Name = EditableName->GetText().ToString();
				InCA.Date = FDateTime::Now().ToString();
				InCA.Lv = 1;
				InCA.SlotPosition = SlotPosition;
				if (InCA.Name.IsEmpty())
				{
					UI_HallMain->PrintLog(TEXT("Name Cannot be empty"));
				}
				else
				{
					
					UI_HallMain->CreateCharacter(InCA);
				}
				//UI_HallMain->PlayRenameOut();
#endif

			}
		}
		//接受完创建后再启用
		//UI_HallMain->ResetCharacterCreatePannel();
	}
}

void UUI_RenameCreate::ClickedCancel()
{
	if (UUI_HallMain* UI_HallMain = GetParents<UUI_HallMain>())
	{
		//编辑角色面板可使用
		if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
		{
			if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotPosition))
			{
				UI_HallMain->ResetCharacterAppearance(InCA);
			}
		}


		UI_HallMain->DestroyCharacter();
		UI_HallMain->PlayRenameOut();
		UI_HallMain->ResetCharacterCreatePannel(true);
	}	
}
