// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Core/UI_KneadFaceBase.h"
#include "UI_CharacterModeling.generated.h"

class UComboBoxString;
class USlider;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_CharacterModeling : public UUI_KneadFaceBase
{
	GENERATED_BODY()

		UPROPERTY(meta = (BindWidget))
		UComboBoxString* ModelingType;
	UPROPERTY(meta = (BindWidget))
		USlider* LegSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* WaistSlider;
	UPROPERTY(meta = (BindWidget))
		USlider* ArmSlider;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* LegValueTexture;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* WaistValueTexture;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ArmValueTexture;

	UPROPERTY(meta = (BindWidget))
		USlider* HeadSlider;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* HeadValueTexture;

	UPROPERTY(meta = (BindWidget))
		USlider* ChestSlider;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ChestValueTexture;
public:
	virtual void NativeConstruct();//构造

	virtual void NativeDestruct();//析构
	//UE自带Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InCAData);
private:
	//更新文本和角色
	void UpdatePawn();
	void UpdateText(UTextBlock* InValueTexture,float Value);
private:
	UFUNCTION()
	void LegValueChanged(float InDeltaTime);
	UFUNCTION()
	void WaistValueChanged(float InDeltaTime);
	UFUNCTION()
	void ArmValueChanged(float InDeltaTime);
	UFUNCTION()
	void SelectModelingType(FString SelectedItem,ESelectInfo::Type SelectionType);

	UFUNCTION()
		void HeadValueChanged(float InDeltaTime);
	UFUNCTION()
		void ChestValueChanged(float InDeltaTime);
	
};
