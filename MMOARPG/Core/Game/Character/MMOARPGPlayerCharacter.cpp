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
			InState->GetCA() = InCA;//给客户端State赋值
			UpdateKneadingBody(InState->GetCA());
			
		}	
	}
}

void AMMOARPGPlayerCharacter::CallServerUpdateKneading_Implementation(int32 InUserID)
{
	//DS服务器
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
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)//模拟
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

	//需要Mesh与骨骼一致变化
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBody()
{

}
