#include "MMOARPGGateRobot.h"
#include "Protocol/HallProtocol.h"
#include "Global/SimpleNetGlobalInfo.h"
#include "SimpleNetManage.h"
#include "UObject/SimpleController.h"

FMMOARPGGateRobot::FMMOARPGGateRobot()
	:GateClient(NULL)
{

}

FMMOARPGGateRobot::~FMMOARPGGateRobot()
{
	if (GateClient)
	{
		FSimpleNetManage::Destroy(GateClient);
		GateClient = NULL;
	}
}

void FMMOARPGGateRobot::RunCharacterAppearanceRequests()
{
	int32 ID = 1;
	SIMPLE_CLIENT_SEND(GateClient, SP_CharacterAppearanceRequests, ID);
}

void FMMOARPGGateRobot::RunLoginToDSServerRequests()
{
	int32 ID = 1;
	int32 InSlotID = 3;

	SIMPLE_CLIENT_SEND(GateClient, SP_LoginToDSServerRequests, ID, InSlotID);
}

void FMMOARPGGateRobot::Init(const FString& Host, const int32 Port)
{
	FSimpleNetGlobalInfo::Get()->Init();

	GateClient = FSimpleNetManage::CreateManage(ESimpleNetLinkState::LINKSTATE_CONNET, ESimpleSocketType::SIMPLESOCKETTYPE_TCP);

	GateClient->NetManageMsgDelegate.BindRaw(this, &FMMOARPGGateRobot::LinkServerInfo);

	if (!GateClient->Init(Host, Port))
	{
		delete GateClient;
		GateClient = NULL;
	}

	GateClient->GetController()->RecvDelegate.AddLambda
	([&](uint32 ProtocolNumber, FSimpleChannel* Channel)
		{
			RecvProtocol(ProtocolNumber,Channel);
		}
	);
}


void FMMOARPGGateRobot::Tick(float DeltaTime)
{
	if (GateClient)
	{
	GateClient->Tick(DeltaTime);
	}
}

void FMMOARPGGateRobot::RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel)
{
	switch (ProtocolNumber)
	{
	case SP_CharacterAppearanceResponses:
	{
		FString CharacterJson;

		//拿到客户端发送的账号
		SIMPLE_PROTOCOLS_RECEIVE(SP_CharacterAppearanceResponses, CharacterJson);
		if (!CharacterJson.IsEmpty())
		{
			RunLoginToDSServerRequests();
		}

		break;
	}
	case SP_LoginToDSServerResponses:
	{
		FSimpleAddr DSServerAddr;
		SIMPLE_PROTOCOLS_RECEIVE(SP_LoginToDSServerResponses, DSServerAddr);

		FString DSAddrString = FSimpleNetManage::GetAddrString(DSServerAddr);
		StartDelegate.ExecuteIfBound();
	}
	default:
		break;
	}
}

void FMMOARPGGateRobot::LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg)
{
	if (Intype == ESimpleNetErrorType::HAND_SHAKE_SUCCESS)
	{
		RunCharacterAppearanceRequests();
	}
}

