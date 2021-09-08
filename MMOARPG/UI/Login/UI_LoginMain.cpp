// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_LoginMain.h"
#include "ThreadManage.h"
#include "UObject/SimpleController.h"
#include "UI_Login.h"
#include "Protocol/LoginProtocol.h"
#include "../../MMOARPGMacroType.h"
#include "../Common/UI_Print.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI_Register.h"


void UUI_LoginMain::NativeConstruct()
{
	Super::NativeConstruct();

	PlayWidgetAnim(TEXT("LoginIn"));
	
	UI_Login->SetParents(this);
	UI_Register->SetParents(this);

	LinkServer();

	//读取账号
	if (!UI_Login->DecryptionFromLocal(FPaths::ProjectDir() / TEXT("User")))
	{
		PrintLog(TEXT("Not account detected"));
	}

	UKismetSystemLibrary::PrintString(this, TEXT("UUI_LoginMain"));

}

void UUI_LoginMain::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUI_LoginMain::SignIn(FString& InAccount,FString& InPassWord)
{
	UKismetSystemLibrary::PrintString(this, TEXT("UUI_LoginMain::SignIn"));
	SEND_DATA(SP_LoginRequests, InAccount, InPassWord);
	//if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
	//{
	//	SIMPLE_CLIENT_SEND(InGameInstance->GetClient(),SP_LoginRequests,InAccount,InPassWord);
	//}
}

void UUI_LoginMain::Register()
{
	UI_Register->RegisterIn();
}

void UUI_LoginMain::Register(FString InRegisterInfo)
{
	UKismetSystemLibrary::PrintString(this, TEXT("UUI_LoginMain::Register"));
	SEND_DATA(SP_RegisterRequests, InRegisterInfo);
}


void UUI_LoginMain::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	//枚举分辨回复字
	switch (ProtocolNumber)
	{
	case SP_LoginRequests:
	{
		break;
	}
	case SP_LoginResponses:
	{
		FString String;
		ELoginType Type = ELoginType::LOGIN_DB_SERVER_ERROR;

		FMMOARPGGateStatus GateStatus;
		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_LoginResponses,Type, String, GateStatus);
		switch (Type)
		{
		case LOGIN_DB_SERVER_ERROR:
				PrintLog(TEXT("Server error"));
				break;
		case LOGIN_SUCCESS:
		{
			PrintLog(TEXT("Login Success"));
			if (UMMOARPGGameInstance* InGameInstance = GetGameInstance<UMMOARPGGameInstance>())
			{
				if (String != TEXT("[]"))
				{
					NetDataAnalysis::StringToUserData(String, InGameInstance->GetUserData());
				}
				//切换服务器
				
				if (GateStatus.GateConnectionNum == INDEX_NONE)
				{
					//等待提示
					PrintLog(TEXT("The number of Servers is full,please wait for a time"));
				}
				else
				{
					InGameInstance->GetGateStatus() = GateStatus;

					if (!UI_Login->EncryptionToLocal(FPaths::ProjectDir() / TEXT("User")))
					{
						PrintLog(TEXT("Password storage failed"));
					}
					else
					{
						PrintLog(TEXT("LOGIN SUCCESS"));		
					}
					PlayWidgetAnim(TEXT("LoginOut"));
					//关闭Login服务器
					if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetChannel())
					{
						//InGameInstance->GetClient()->Close();
						InGameInstance->GetClient()->GetChannel()->DestroySelf();
					}
					//协程
					GThread::Get()->GetCoroutines().BindLambda(1.f,
						[&]()
						{
							UGameplayStatics::OpenLevel(GetWorld(), TEXT("HallMap"));
						});
				}
			}
			break;
		}
		case LOGIN_ACCOUNT_WRONG:
				PrintLog(TEXT("Account does not exist"));
				break;
		case LOGIN_WRONG_PASSWORD:
				PrintLog(TEXT("PassWoed verification failed"));
				break;
		}

		break;
	}
	case SP_RegisterResponses:
	{
		ERegistrationType Type = ERegistrationType::SERVER_BUG_WRONG;
		FSimpleAddrInfo AddrInfo;
		//拿到注册结果
		SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterResponses, AddrInfo, Type);
		switch (Type)
		{
		case ACCOUNT_AND_EMAIL_PERTITION_ERROR:
			PrintLog(TEXT("account_and_email_pertition_error"));

			//协程,把注册界面再次展开
			GThread::Get()->GetCoroutines().BindLambda(0.5f,
				[&]()
				{
					Register();
					//UI_Register->RegisterIn();
				});
#if 0
			{
				//支持指针和传参，不支持引用（不安全）
				int32 xx = 1;
				UObject* Object;
				GThread::Get()->GetCoroutines().BindLambda(0.5f,
					[&](UObject* InObject,int32 Inxx)
					{
						Register();
						//UI_Register->RegisterIn();
					},Object,xx);
			}
#endif


			break;
		case PLAYER_REGISTRATION_SUCCESS:
		{
			PrintLog(TEXT("player_registration_success"));
			break;
		}		
		case SERVER_BUG_WRONG:
			PrintLog(TEXT("server_bug_wrong"));

			break;
		default:
			break;
		}
	}
	}
}

void UUI_LoginMain::LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg)
{
	if (Intype==ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		UI_LinkWidget->SetVisibility(ESlateVisibility::Collapsed);
		//握手成功，UI塌陷
	}
}
