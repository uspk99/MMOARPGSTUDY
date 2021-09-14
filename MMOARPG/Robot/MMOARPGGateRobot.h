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

	//这个协议主要是为了在网关那边注册id
	void RunCharacterAppearanceRequests();
	//这个协议主要是为了登录 在服务器注册一个玩家
	void RunLoginToDSServerRequests();

	void Init(const FString& Host, const int32 Port);

	void Tick(float DeltaTime);

	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

	void LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg);
private:	
	FSimpleNetManage* GateClient;


};