#pragma once
#include "SimpleNetManage.h"
#include "MMOARPGType.h"
#include "Delegates/Delegate.h"

class FMMOARPGGateRobot
{
public:
	FMMOARPGGateRobot();
	virtual ~FMMOARPGGateRobot();

	FSimpleDelegate StartDelegate;

	//���Э����Ҫ��Ϊ���������Ǳ�ע��id
	void RunCharacterAppearanceRequests();
	//���Э����Ҫ��Ϊ�˵�¼ �ڷ�����ע��һ�����
	void RunLoginToDSServerRequests();

	void Init(const FString& Host, const int32 Port);

	void Tick(float DeltaTime);

	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

	void LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg);
private:	
	FSimpleNetManage* GateClient;


};