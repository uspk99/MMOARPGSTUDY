// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Core/UI_Base.h"
#include "UI_PartnerList.generated.h"

class UVerticalBox;
class UUI_PartnerInfo;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_PartnerList : public UUI_Base
{
	GENERATED_BODY()

		UPROPERTY(meta=(Bindwidget))
		UVerticalBox* CharacterList;

		//±©Â¶¸øÀ¶Í¼
		UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUI_PartnerInfo> UI_PartnerInfoClass;
	

public:
	virtual void NativeConstruct();

	virtual void NativeDestruct();

public:
	void Init();
	//±éÀúslot
	void CallCharacterListByPredicate(TFunction<bool(UUI_PartnerInfo* Info)> InFun);
};
