// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterCreatePannel.h"
#include "Components/TextBlock.h"
#include "UI_CharacterButton.h"
#include "UI_KneadFace.h"
#include "Components/ScrollBoxSlot.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "MMOARPG/Core/Hall/HallPawn.h"
#include <MMOARPG/Core/Hall/Character/CharacterStage.h>
#include "../UI_HallMain.h"

void UUI_CharacterCreatePannel::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPositon = 0;
	//CreateCharacterButton();
}

void UUI_CharacterCreatePannel::NativeDestruct()
{
	Super::NativeDestruct();
}

UUI_KneadFace* UUI_CharacterCreatePannel::CreateKneadFace()
{
	List->ClearChildren();
	if (UI_KneadFaceClass)
	{
		if (UUI_KneadFace* InKneadFace =CreateWidget<UUI_KneadFace>(GetWorld(),UI_KneadFaceClass))
		{
			InKneadFace->SetParents(this);
			if (UScrollBoxSlot* InScrollBoxSlot = Cast<UScrollBoxSlot>(List->AddChild(InKneadFace)))
			{
				InScrollBoxSlot->SetPadding(5.f);
			}
			return InKneadFace;
		}
	}
	return nullptr;
}

void UUI_CharacterCreatePannel::CreateCharacterButton()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		InitCharacterButtons(InState->GetCharacterAppearances());
	}
}

void UUI_CharacterCreatePannel::InitCharacterButtons(FCharacterAppearances& InCAs)
{
	List->ClearChildren();
	//初始化添加角色按钮
	if (UI_CharacterButtonClass)
	{
		for (int32 i = 0; i < 4.; i++)
		{
			if (UUI_CharacterButton* InCharacterButton = CreateWidget<UUI_CharacterButton>(GetWorld(), UI_CharacterButtonClass))
			{
				if (UScrollBoxSlot *InScrollBoxSlot=Cast<UScrollBoxSlot>( List->AddChild(InCharacterButton)))
				{
					//设置填充区
					InScrollBoxSlot->SetPadding(10.f);
				}
				//放下面
				InCharacterButton->SetParents(this);
				InCharacterButton->SetSlotPosition(i);

				//数据初始化
				if (FMMOARPGCharacterAppearance *InCharacterAppearance =
					InCAs.FindByPredicate(
						[&](const FMMOARPGCharacterAppearance& InCharacterAppearance)
						{
							return InCharacterAppearance.SlotPosition == i;
						}))
				{
					InCharacterButton->InitCharacterButton(*InCharacterAppearance);
				}
			}
		}
	}
}

ACharacterStage* UUI_CharacterCreatePannel::CreateCharacter()
{
	if (CharacterStageClass)
	{
		if (AHallPawn* InPawn = GetPawn<AHallPawn>())
		{
			if (InPawn->CharacterStage)
			{
				InPawn->CharacterStage->Destroy();
			}
			InPawn->CharacterStage =
				GetWorld()->SpawnActor<ACharacterStage>(CharacterStageClass, SpawnPoint, FRotator::ZeroRotator);
			return InPawn->CharacterStage;
		}
	}
	return nullptr;
}

void UUI_CharacterCreatePannel::SpawnCharacter(const int32 InSlotPos)
{

	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (FMMOARPGCharacterAppearance* InCAData = InState->GetCharacterAppearances().FindByPredicate(
			[&](const FMMOARPGCharacterAppearance& InCA)
			{
				return	InCA.SlotPosition == InSlotPos;
			}))
		{
			SpawnCharacter(InCAData);
		}
	}
}

void UUI_CharacterCreatePannel::SpawnCharacter()
{
	SpawnCharacter(SlotPositon);
}

void UUI_CharacterCreatePannel::SpawnCharacter(const FMMOARPGCharacterAppearance* InACData)
{
	if (InACData)
	{
		if (CharacterStageClass)
		{
			if (ACharacterStage * InCharacter = CreateCharacter())
			{
				//生成角色时，传入Slot 并更新外形
				InCharacter->SetSlotID(InACData->SlotPosition);
				InCharacter->UpdateKneadingBody(*InACData);
			}	
		}
	}
}

void UUI_CharacterCreatePannel::SetCurrentSlotPositon(const int32 InNewPos)
{
	SlotPositon = InNewPos;
	HighlightDefaultSelection(SlotPositon);
}

void UUI_CharacterCreatePannel::HighlightDefaultSelection(const int32 InNewPos)
{
	FindByPerdicateInList<UUI_CharacterButton>(
		[InNewPos](UUI_CharacterButton* InButton)->bool
		{
			if (InNewPos==InButton->GetSlotPositon())
			{
				InButton->SetHighLight(true);
			}
			else
			{
				//让其他的角色槽暗下去
				InButton->SetHighLight(false);
			}
			return false;
		});
}

UUI_CharacterButton* UUI_CharacterCreatePannel::GetHighlightButton()
{

	return FindByPerdicateInList<UUI_CharacterButton>(
		[](UUI_CharacterButton* InButton)->bool
		{
			if (InButton->IsHighLight())
			{
				return true;
			}
			return false;
		});
}

void UUI_CharacterCreatePannel::JumpDSServer(int32 InSlotID)
{
	if (UUI_HallMain* InHallMain = GetParents<UUI_HallMain>())
	{
		InHallMain->JumpDSServer(InSlotID);
	}
}
