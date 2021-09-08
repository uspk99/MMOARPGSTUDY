// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "MMOARPGType.h"
#include "UI_KneadFaceBase.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_KneadFaceBase : public UUI_Base
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();//����

	virtual void NativeDestruct();//����

	virtual void InitKneadFace(const FMMOARPGCharacterAppearance* InCAData){}
	
};
