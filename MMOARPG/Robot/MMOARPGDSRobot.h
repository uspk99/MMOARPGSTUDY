#pragma once
#include "SimpleNetManage.h"
#include "MMOARPGType.h"


class FMMOARPGDSRobot
{
public:
	FMMOARPGDSRobot();

	void Init(const FString& Host, const int32 Port);

	void Tick(float DeltaTime);

private:
	FSimpleNetManage* DSClient;

};