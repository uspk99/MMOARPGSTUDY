// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_PartnerInfo.generated.h"

class UImage;
class UTextBlock;
class UProgressBar;
class UCheckBox;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_PartnerInfo : public UUI_Base
{
	friend class UUI_PartnerList;
	GENERATED_BODY()

		UPROPERTY(meta = (Bindwidget))
		UImage* CharacterIcon;
	UPROPERTY(meta = (Bindwidget))
		UTextBlock* CharacterName;
	UPROPERTY(meta = (Bindwidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (Bindwidget))
		UProgressBar* ManaBar;
	UPROPERTY(meta = (Bindwidget))
		UCheckBox* ClickedCheckBox;
	
public:
	UUI_PartnerInfo(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct();

	virtual void NativeDestruct();

	int32 GetCharacterID() { return CharacterID; }

	void ShowSelected(bool bShow);
public:
	UFUNCTION()
	void OnClickedCharacter(bool bClicked);

protected:
	void SetCharacterID(int32 InCharacterID) { CharacterID = InCharacterID; }

	int32 CharacterID;
};
