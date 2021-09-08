// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_KneadFaceBase.h"
#include "MMOARPGType.h"
#include "UI_KneadFace.generated.h"

class UCheckBox;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_KneadFace : public UUI_KneadFaceBase
{
	GENERATED_BODY()
	UPROPERTY(meta =(BindWidget))
		UCheckBox* Body;
	UPROPERTY(meta = (BindWidget))
		UCheckBox* Appearance;
	UPROPERTY(meta = (BindWidget))
		UCheckBox* Talent;
	UPROPERTY(meta = (BindWidget))
		UWidgetSwitcher* FacePanel;
public:
	virtual void NativeConstruct();//¹¹Ôì

	virtual void NativeDestruct();//Îö¹¹

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InCA);
private:
	UFUNCTION()
	void OnClickedBody(bool bReleaseChildren);
	UFUNCTION()
	void OnClickedAppearance(bool bReleaseChildren);
	UFUNCTION()
	void OnClickedTalent(bool bReleaseChildren);

};
