// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_MainBase.h"
#include "../Common/UI_Print.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "../../MMOARPGMacroType.h"

void UUI_MainBase::NativeConstruct()
{
	Super::NativeConstruct();

}

void UUI_MainBase::NativeDestruct()
{
	Super::NativeDestruct();

	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetController())
		{
			InGameInstance->GetClient()->GetController()->RecvDelegate.Remove(RecvDelegate);
		}
	}
}

void UUI_MainBase::PrintLog(const FString& InMsg)
{
	PrintLog(FText::FromString(InMsg));
}

void UUI_MainBase::PrintLog(const FText& InMsg)
{
	//播放动画
	UI_Print->PlayTextAnim();

	UI_Print->SetText(InMsg);
}

void UUI_MainBase::BindClientRcv()
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

void UUI_MainBase::LinkServer(const FSimpleAddr& InAddr)
{
	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);

			InGameInstance->LinkServer(InAddr);

			BindClientRcv();
		}
	}
}

void UUI_MainBase::LinkServer()
{
	//创建客户端
	if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	{
		InGameInstance->CreateClient();
		if (InGameInstance->GetClient())
		{
			InGameInstance->GetClient()->NetManageMsgDelegate.BindUObject(this, &UUI_MainBase::LinkServerInfo);

			InGameInstance->LinkServer();

			BindClientRcv();
		}
	}
}

void UUI_MainBase::LinkServerInfo(ESimpleNetErrorType InType, const FString& InMsg)
{

}