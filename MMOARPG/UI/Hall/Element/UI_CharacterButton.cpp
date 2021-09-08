// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterButton.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Math/Vector.h"
#include "../../../Core/Hall/Character/CharacterStage.h"
#include "../../../Core/Hall/HallPawn.h"
#include "UI_CharacterCreatePannel.h"
#include "../UI_HallMain.h"
#include "Internationalization/Text.h"
#include "../../../Core/Hall/HallPlayerState.h"
#include "Kismet/GameplayStatics.h"


void UUI_CharacterButton::NativeConstruct()
{
	Super::NativeConstruct();

	SlotPositon = INDEX_NONE;

	CharacterButton->OnReleased.AddDynamic(this,&UUI_CharacterButton::ClickedCharacter);
	//Ĭ����ʾ��ʽ
	CreateShowIcon->SetVisibility(ESlateVisibility::Visible);
	CharacterInfo->SetVisibility(ESlateVisibility::Collapsed);

	//slatecolorת����linearcolor
	DefaultColor = CharacterButton->WidgetStyle.Normal.TintColor.GetSpecifiedColor();
}

void UUI_CharacterButton::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterButton::SetSlotPosition(const int32 InNewPos)
{
	SlotPositon = InNewPos;
}

void UUI_CharacterButton::InitCharacterButton(const FMMOARPGCharacterAppearance& InCA)
{
	if (InCA.SlotPosition!=INDEX_NONE)
	{
		CreateShowIcon->SetVisibility(ESlateVisibility::Collapsed);
		CharacterInfo->SetVisibility(ESlateVisibility::Visible);

		CharacterName->SetText(FText::FromString(InCA.Name));
		Date->SetText(FText::FromString(InCA.Date));
	}
}

void UUI_CharacterButton::SetHighLight(bool bHighLight)
{
	if (bHighLight)
	{
		CharacterButton->WidgetStyle.Normal.TintColor = HighlightColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
	else
	{
		CharacterButton->WidgetStyle.Normal.TintColor = DefaultColor;
		CharacterButton->SetStyle(CharacterButton->WidgetStyle);
	}
}

bool UUI_CharacterButton::IsHighLight()
{
	return CharacterButton->WidgetStyle.Normal.TintColor == HighlightColor;

}

void UUI_CharacterButton::JumpDSServer()
{
	if (UUI_CharacterCreatePannel* UI_CharacterCreatePannel = GetParents<UUI_CharacterCreatePannel>())
	{
		UI_CharacterCreatePannel->JumpDSServer(SlotPositon);
	}
}

void UUI_CharacterButton::ClickedCharacter()
{
	if (AHallPlayerState* InState = GetPlayerState<AHallPlayerState>())
	{
		if (UUI_CharacterCreatePannel* UI_CharacterCreatePannel = GetParents<UUI_CharacterCreatePannel>())
		{
			//������ɫ
			if (!InState->IsCharacterExistInSlot(SlotPositon))
			{
				//���ɽ�ɫͬʱд������
				if (ACharacterStage* InCS=UI_CharacterCreatePannel->CreateCharacter())
				{
					if (InState->GetCurrentTmpCreateCharacter())
					{
						//���ý�ɫ����type
						InState->GetCurrentTmpCreateCharacter()->Reset();
						InState->GetCurrentTmpCreateCharacter()->SlotPosition = this->SlotPositon;
					}
					InCS->SetSlotID(SlotPositon);
					InCS->UpdateKneadingBody();
				}

				//UI_CharacterCreatePannel->CreateCharacter();
				//�л�
				UI_CharacterCreatePannel->CreateKneadFace();
				if (UUI_HallMain* UI_HallMain = UI_CharacterCreatePannel->GetParents<UUI_HallMain>())
				{
					UI_HallMain->PlayRenameIn();
					UI_HallMain->SetSlotPosition(SlotPositon);
				}			
			}
			//�ж��Ƿ��Ѿ�ѡ���˽�ɫ
			else if(UI_CharacterCreatePannel->GetHighlightButton()==this)
			{
				//��DS
				JumpDSServer();
			}
			//δѡ���˽�ɫ,���ø���
			else
			{
				UI_CharacterCreatePannel->GetHighlightButton()->SetHighLight(false);
				SetHighLight(true);
				if (FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotPositon))
				{
					UI_CharacterCreatePannel->SpawnCharacter(InCA);
				}

			}
			UI_CharacterCreatePannel->SetCurrentSlotPositon(SlotPositon);
		}

	}

}
