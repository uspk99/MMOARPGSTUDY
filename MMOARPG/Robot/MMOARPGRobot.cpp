#include "MMOARPGRobot.h"

FMMOARPGRobot::FMMOARPGRobot()
{

}

void FMMOARPGRobot::InitGate(const FString& Host, const int32 Port)
{
	GateRobot.Init(Host, Port);
}

void FMMOARPGRobot::InitDS(const FString& Host, const int32 Port)
{
	DSRobot.Init(Host, Port);
}

void FMMOARPGRobot::Tick(float DeltaTime)
{
	DSRobot.Tick(DeltaTime);
	GateRobot.Tick(DeltaTime);
}

void FMMOARPGRobot::RunRobot()
{

}