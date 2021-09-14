// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "MMOARPGHUD.h"
#include "../../MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "SimpleNetManage.h"
#include "UObject/SimpleController.h"
#include "MMOARPGPlayerState.h"
#include "MMOARPGGameState.h"
#include "Character/MMOARPGCharacter.h"
#include "ThreadManage.h"
#include "Character/MMOARPGPlayerCharacter.h"
#include "SimpleAdvancedAnimationBPLibrary.h"
#include "Protocol/GameProtocol.h"
#include "../../MMOARPGMacroType.h"
#include "Core/MethodUnit.h"
#include "MMOARPGPlayerController.h"



AMMOARPGGameMode::AMMOARPGGameMode()
{
	HUDClass = AMMOARPGHUD::StaticClass();
	PlayerStateClass = AMMOARPGPlayerState::StaticClass();
	GameStateClass = AMMOARPGGameState::StaticClass();
	PlayerControllerClass = AMMOARPGPlayerController::StaticClass();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMMOARPGGameMode::BeginPlay()
{
	Super::BeginPlay();
	LinkServer();
}

void AMMOARPGGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	Super::EndPlay(EndPlayReason);

	USimpleAdvancedAnimationBPLibrary::Destroy();

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(RecvDelegate);
		}
	}
}

void AMMOARPGGameMode::LinkServer()
{
	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &AMMOARPGGameMode::LinkServerInfo);
			//测试时固定地址端口
			InGameInstance->LinkServer(TEXT("127.0.0.1"),11231);

			BindClientRcv();
		}
	}
}

void AMMOARPGGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMMOARPGGameMode::BindClientRcv()
{
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			RecvDelegate = InGameInstance->GetClient()->GetController()->RecvDelegate.AddLambda(
				[&](uint32 ProtocolNumber, FSimpleChannel* Channel)
				{
					this->RecvProtocol(ProtocolNumber, Channel);
				});
		}
		else
		{
			GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
		}
	}
	else
	{
		GThread::Get()->GetCoroutines().BindLambda(0.5f, [&]() { BindClientRcv(); });
	}
}

void AMMOARPGGameMode::LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg)
{

}

void AMMOARPGGameMode::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case SP_UpdateLoginCharacterInfoResponses:
	{
		int32 UserID = INDEX_NONE;
		FString CAString;
		SIMPLE_PROTOCOLS_RECEIVE(SP_UpdateLoginCharacterInfoResponses, UserID, CAString);
		if (UserID!=INDEX_NONE && !CAString.IsEmpty())
		{
			FMMOARPGCharacterAppearance CA;
			NetDataAnalysis::StringToCharacterAppearances(CAString, CA);

			//遍历寻找对应Id玩家
			MethodUnit::ServerCallAllPlayer<AMMOARPGPlayerCharacter>(GetWorld(),
				[&](AMMOARPGPlayerCharacter* InPawn)->MethodUnit::EServerCallType
				{
					if (InPawn->GetUserID() == UserID)
					{
						InPawn->UpdateKneadingBody(CA);
						InPawn->CallUpdateKneadingBody(CA);
						return MethodUnit::EServerCallType::PROGRESS_COMPLETE;
					}
					return MethodUnit::EServerCallType::INPROGRESS;
				});
		}

		break;
	}
	default:
		break;
	}
}

//DS Server
void AMMOARPGGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GThread::Get()->GetCoroutines().BindLambda(1.f, [&](APlayerController* InNewPlayer)
		{
			if (InNewPlayer)
			{
				//if (AMMOARPGPlayerCharacter* InPawn = InNewPlayer->GetPawn<AMMOARPGPlayerCharacter>())
				//{
				//	if (AMMOARPGGameState* InGameState = GetGameState<AMMOARPGGameState>())
				//	{
				//		if (FCharacterAnimTable* InAnimTable = InGameState->GetCharacterAnimTable(InPawn->GetID()))
				//		{
				//			InPawn->AnimTable = InAnimTable;
				//		}
				//	}
				//}
			}
		},NewPlayer);
}

void AMMOARPGGameMode::LoginCharacterUpdateKneadingRequest(int32 InUserID)
{
	SEND_DATA(SP_UpdateLoginCharacterInfoRequests, InUserID);
}

