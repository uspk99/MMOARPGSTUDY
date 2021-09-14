// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGPlayerCharacter.h"
#include "../../../MMOARPGGameInstance.h"
#include "../MMOARPGGameMode.h"
#include "ThreadManage.h"
#include "../../../MMOARPGMacroType.h"
#include "../MMOARPGPlayerState.h"

void AMMOARPGPlayerCharacter::UpdateKneadingRequest()
{
	if (UMMOARPGGameInstance* InGameInstance = GetWorld()->GetGameInstance<UMMOARPGGameInstance>())
	{
#if UE_MMOARPG_DEBUG_DS
		CallServerUpdateKneading(1);
#endif

	CallServerUpdateKneading(InGameInstance->GetUserData().ID);
	}
}

void AMMOARPGPlayerCharacter::CallUpdateKneadingBodyOnClient_Implementation(const FMMOARPGCharacterAppearance& InCA)
{
	UpdateKneadingBody(InCA);

	if (GetLocalRole()==ENetRole::ROLE_AutonomousProxy)
	{
		if (AMMOARPGPlayerState* InState = GetPlayerState<AMMOARPGPlayerState>())
		{
			InState->GetCA() = InCA;//���ͻ���State��ֵ
			UpdateKneadingBody(InState->GetCA());
			
		}	
	}
}

void AMMOARPGPlayerCharacter::CallServerUpdateKneading_Implementation(int32 InUserID)
{
	//DS������
	if (AMMOARPGGameMode* InGameMode = GetWorld()->GetAuthGameMode<AMMOARPGGameMode>())
	{
		UserID = InUserID;
		InGameMode->LoginCharacterUpdateKneadingRequest(InUserID);
	}
}

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());

	if (GetLocalRole()==ENetRole::ROLE_AutonomousProxy)
	{
#if !UE_MMOARPG_DEBUG_DS
		UpdateKneadingRequest();
#endif
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)//ģ��
	{

	}
}

void AMMOARPGPlayerCharacter::UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadtSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);

	//��ҪMesh�����һ�±仯
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBody()
{

}
