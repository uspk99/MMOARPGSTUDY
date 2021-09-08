// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_CharacterModeling.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "../../../../Core/Hall/HallPlayerState.h"
#include "../../../../Core/Hall/HallPawn.h"
#include "../../../../Core/Hall/Character/CharacterStage.h"


void UUI_CharacterModeling::NativeConstruct()
{
	Super::NativeConstruct();
	//绑定滑动条
	LegSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::LegValueChanged);
	WaistSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::WaistValueChanged);
	ArmSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ArmValueChanged);
	HeadSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::HeadValueChanged);
	ChestSlider->OnValueChanged.AddDynamic(this, &UUI_CharacterModeling::ChestValueChanged);
	//绑定选择
	ModelingType->OnSelectionChanged.AddDynamic(this, &UUI_CharacterModeling::SelectModelingType);

}

void UUI_CharacterModeling::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_CharacterModeling::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_CharacterModeling::InitKneadFace(const FMMOARPGCharacterAppearance* InCAData)
{
	if (InCAData)//角色无误则设置参数
	{
		auto GetKFValue = [](float InValue)->float
		{
			return InValue / 10.f;
		};
		//InCAData->LegSize = LegSlider->GetValue() * 10.f;
		//InCAData->WaistSize = WaistSlider->GetValue() * 10.f;
		//InCAData->ArmSize = ArmSlider->GetValue() * 10.f;
		LegSlider->SetValue(GetKFValue(InCAData->LegSize));
		WaistSlider->SetValue(GetKFValue(InCAData->WaistSize));
		ArmSlider->SetValue(GetKFValue(InCAData->ArmSize));
		HeadSlider->SetValue(GetKFValue(InCAData->HeadSize));
		ChestSlider->SetValue(GetKFValue(InCAData->ChestSize));

		LegValueChanged(GetKFValue(InCAData->LegSize));
		WaistValueChanged(GetKFValue(InCAData->WaistSize));
		ArmValueChanged(GetKFValue(InCAData->ArmSize));
		HeadValueChanged(GetKFValue(InCAData->HeadSize));
		ChestValueChanged(GetKFValue(InCAData->ChestSize));

		UpdatePawn();
	}
}

void UUI_CharacterModeling::UpdatePawn()
{
	if (AHallPawn* InPawn = GetPawn<AHallPawn>())
	{
		if (AHallPlayerState* InState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AHallPlayerState>())
		{
			if (InPawn->CharacterStage)
			{
				//获取目前角色slot
				int32 SlotID = InPawn->CharacterStage->GetSlotID();
				FMMOARPGCharacterAppearance* InCA = InState->GetCharacterCA(SlotID);
				if (!InCA)
				{
					if (auto* CurrentTmpCreateCharacter =InState->GetCurrentTmpCreateCharacter())
					{
						//判断目前角色和state里临时角色一致
						if (CurrentTmpCreateCharacter->SlotPosition == SlotID)
						{
							InCA = CurrentTmpCreateCharacter;
						}
					}
				}
				if (InCA)//角色无误则设置参数
				{
					InCA->LegSize = LegSlider->GetValue() * 10.f;
					InCA->WaistSize = WaistSlider->GetValue() * 10.f;
					InCA->ArmSize = ArmSlider->GetValue() * 10.f;
					InCA->HeadSize = HeadSlider->GetValue() * 10.f;
					InCA->ChestSize = ChestSlider->GetValue() * 10.f;
				}
				InPawn->CharacterStage->UpdateKneadingBody();
			}
		}

	}
}

void UUI_CharacterModeling::UpdateText(UTextBlock* InValueTexture, float Value)
{
	if (InValueTexture)
	{
		int32 InValue = Value * 10;
		InValueTexture->SetText(FText::FromString(FString::Printf(TEXT("%02d"), InValue)));
	}
}

void UUI_CharacterModeling::LegValueChanged(float InDeltaTime)
{
	UpdateText(LegValueTexture, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::WaistValueChanged(float InDeltaTime)
{
	UpdateText(WaistValueTexture, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::ArmValueChanged(float InDeltaTime)
{
	UpdateText(ArmValueTexture, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::SelectModelingType(FString SelectedItem, ESelectInfo::Type SelectionType)
{

}

void UUI_CharacterModeling::HeadValueChanged(float InDeltaTime)
{
	UpdateText(HeadValueTexture, InDeltaTime);
	UpdatePawn();
}

void UUI_CharacterModeling::ChestValueChanged(float InDeltaTime)
{
	UpdateText(ChestValueTexture, InDeltaTime);
	UpdatePawn();
}
