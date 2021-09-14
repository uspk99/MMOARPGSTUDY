// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_GameMain.h"
#include "ThreadManage.h"

#if UE_MMOARPG_DEBUG_DS
#include "../../Core/Game/Character/MMOARPGPlayerCharacter.h"
#endif

#define LOCTEXT_NAMESPACE "UUI_GameMain"

void UUI_GameMain::NativeConstruct()
{
	Super::NativeConstruct();

#if UE_MMOARPG_DEBUG_DS
	Robot.InitGate("127.0.0.1", 11222);

	if (AMMOARPGPlayerCharacter* InPawn = GetPawn<AMMOARPGPlayerCharacter>())
	{
		Robot.GetGateRobot().StartDelegate.BindUObject(InPawn, &AMMOARPGPlayerCharacter::UpdateKneadingRequest);
	}
#endif

}

void UUI_GameMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_GameMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

#if UE_MMOARPG_DEBUG_DS
	Robot.Tick(InDeltaTime);
#endif


}

void UUI_GameMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case 101:
	{
		break;
	}
	default:
		break;
	}
}

void UUI_GameMain::LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg)
{
	if (Intype==ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{

	}
}

#undef LOCTEXT_NAMESPACE