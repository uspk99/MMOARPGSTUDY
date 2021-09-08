// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_KneadFace.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"

void UUI_KneadFace::NativeConstruct()
{
	Super::NativeConstruct();

	Body->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedBody);
	Appearance->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedAppearance);
	Talent->OnCheckStateChanged.AddDynamic(this, &UUI_KneadFace::OnClickedTalent);
	//Ä¬ÈÏBodyÇÃ»÷×´Ì¬
	Body->SetCheckedState(ECheckBoxState::Checked);
}

void UUI_KneadFace::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_KneadFace::InitKneadFace(const FMMOARPGCharacterAppearance* InCA)
{
	for (int32 i=0;i<FacePanel->GetNumWidgets();i++)
	{
		if (UUI_KneadFaceBase* InKFB =Cast<UUI_KneadFaceBase>(FacePanel->GetWidgetAtIndex(i)))
		{
			InKFB->InitKneadFace(InCA);
		}
	}
}

void UUI_KneadFace::OnClickedBody(bool bReleaseChildren)
{
	//ÉèÖÃÇÃ»÷×´Ì¬
	Body->SetCheckedState(ECheckBoxState::Checked);
	Appearance->SetCheckedState(ECheckBoxState::Unchecked);
	Talent->SetCheckedState(ECheckBoxState::Unchecked);
	//ÇÐ»»½çÃæ
	FacePanel->SetActiveWidgetIndex(0);
}

void UUI_KneadFace::OnClickedAppearance(bool bReleaseChildren)
{
	//ÉèÖÃÇÃ»÷×´Ì¬
	Body->SetCheckedState(ECheckBoxState::Unchecked);
	Appearance->SetCheckedState(ECheckBoxState::Checked);
	Talent->SetCheckedState(ECheckBoxState::Unchecked);

	FacePanel->SetActiveWidgetIndex(1);
}

void UUI_KneadFace::OnClickedTalent(bool bReleaseChildren)
{
	//ÉèÖÃÇÃ»÷×´Ì¬
	Body->SetCheckedState(ECheckBoxState::Unchecked);
	Appearance->SetCheckedState(ECheckBoxState::Unchecked);
	Talent->SetCheckedState(ECheckBoxState::Checked);

	FacePanel->SetActiveWidgetIndex(2);
}
