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

			MethodUnit::ServerCallAllPlayerController<AMMOARPGPlayerController>(GetWorld(),
				[&](AMMOARPGPlayerController* InController)->MethodUnit::EServerCallType
				{
					if (AMMOARPGPlayerCharacter* InPawn = InController->GetPawn<AMMOARPGPlayerCharacter>())
					{
						if (InPawn->GetUserID() == UserID)
						{
							if (AMMOARPGPlayerState* InPlayerState= InController->GetPlayerState<AMMOARPGPlayerState>())
							{
								NetDataAnalysis::StringToCharacterAppearances(CAString, InPlayerState->GetCA());
								InPawn->UpdateKneadingBody(InPlayerState->GetCA());
								InPawn->CallUpdateKneadingBodyOnClient(InPlayerState->GetCA());
							}
							return MethodUnit::EServerCallType::PROGRESS_COMPLETE;
						}
					}
					return MethodUnit::EServerCallType::INPROGRESS;
				});
		}
		break;
	}
	case SP_GetCharacterDataResponses:
	{
		int32 UserID = INDEX_NONE;
		//int32 CharacerID =INDEX_NONE;
		FString CDJsonString;
		SIMPLE_PROTOCOLS_RECEIVE(SP_GetCharacterDataResponses, UserID, CDJsonString);

		if (UserID!=INDEX_NONE && !CDJsonString.IsEmpty())
		{
			MethodUnit::ServerCallAllPlayerController<AMMOARPGPlayerController>(GetWorld(),
				[&](AMMOARPGPlayerController* InController)->MethodUnit::EServerCallType
				{
					if (AMMOARPGPlayerCharacter* InPawn = InController->GetPawn<AMMOARPGPlayerCharacter>())
					{
						if (InPawn->GetUserID() == UserID)
						{	//Json转回属性集
							FMMOARPGCharacterAttribute CharacterAttribute;
							NetDataAnalysis::StringToCharacterAttribute(CDJsonString,CharacterAttribute);

							InPawn->UpdateCharacterAttribute(CharacterAttribute);

							return MethodUnit::EServerCallType::PROGRESS_COMPLETE;
						}
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

void AMMOARPGGameMode::GetCharacterDataRequests(int32 InUserID, int32 InCharacterID)
{
	SEND_DATA(SP_GetCharacterDataRequests,InUserID, InCharacterID);
}

