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

	//��ȡ�˺�
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
	//ö�ٷֱ�ظ���
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
		//�õ��ͻ��˷��͵��˺�
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
				//�л�������
				
				if (GateStatus.GateConnectionNum == INDEX_NONE)
				{
					//�ȴ���ʾ
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
					//�ر�Login������
					if (InGameInstance->GetClient() && InGameInstance->GetClient()->GetChannel())
					{
						//InGameInstance->GetClient()->Close();
						InGameInstance->GetClient()->GetChannel()->DestroySelf();
					}
					//Э��
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
		//�õ�ע����
		SIMPLE_PROTOCOLS_RECEIVE(SP_RegisterResponses, AddrInfo, Type);
		switch (Type)
		{
		case ACCOUNT_AND_EMAIL_PERTITION_ERROR:
			PrintLog(TEXT("account_and_email_pertition_error"));

			//Э��,��ע������ٴ�չ��
			GThread::Get()->GetCoroutines().BindLambda(0.5f,
				[&]()
				{
					Register();
					//UI_Register->RegisterIn();
				});
#if 0
			{
				//֧��ָ��ʹ��Σ���֧�����ã�����ȫ��
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
		//���ֳɹ���UI����
	}
}
