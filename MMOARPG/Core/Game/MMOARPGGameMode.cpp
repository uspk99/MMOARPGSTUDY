// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMOARPGGameMode.h"
#include "MMOARPGCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MMOARPGHUD.h"
#include "../../MMOARPGGameInstance.h"
#include "ThreadManage.h"
#include "SimpleNetManage.h"
#include "UObject/SimpleController.h"


AMMOARPGGameMode::AMMOARPGGameMode()
{
	HUDClass = AMMOARPGHUD::StaticClass();
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

}

