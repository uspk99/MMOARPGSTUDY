// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Core/UI_MainBase.h"
#include "../../MMOARPGMacroType.h"
#if UE_MMOARPG_DEBUG_DS
#include "../../Robot/MMOARPGRobot.h"
#endif
#include "UI_GameMain.generated.h"


/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_GameMain : public UUI_MainBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();//构造

	virtual void NativeDestruct();//析构

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
protected:

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);


	virtual void LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg);

protected:
#if UE_MMOARPG_DEBUG_DS
	FMMOARPGRobot Robot;//只是为了测试
#endif // UE_MMOARPG_DEBUG_DS

};
