// Fill out your copyright notice in the Description page of Project Settings.


#include "KneadingInterface.h"

IKneadingInterface::IKneadingInterface()
	:LegSize(0.f)
	,WaistSize(0.f)
	,ArmSize(0.f)
{

}

// Add default functionality here for any IKneadingInterface functions that are not pure virtual.

void IKneadingInterface::SetLegSize(float InLegSize)
{
	LegSize = InLegSize;
}

void IKneadingInterface::SetWaistSize(float InWaistSize)
{
	WaistSize = InWaistSize;
}

void IKneadingInterface::SetArmSize(float InArmSize)
{
	ArmSize = InArmSize;
}

void IKneadingInterface::SetHeadtSize(float InHeadSize)
{
	HeadSize = InHeadSize;
}

void IKneadingInterface::SetChestSize(float InChestSize)
{
	ChestSize = InChestSize;
}

float IKneadingInterface::GetLegSize()
{
	return LegSize;
}

float IKneadingInterface::GetWaistSize()
{
	return WaistSize;
}

float IKneadingInterface::GetArmSize()
{
	return ArmSize;
}

float IKneadingInterface::GetHeadSize()
{
	return HeadSize;
}

float IKneadingInterface::GetChestSize()
{
	return ChestSize;
}

void IKneadingInterface::SetMeshPostion(USceneComponent* InMesh)
{
	//根据身高变化给模型增加Z轴偏移
	if (InMesh)
	{
		FVector ComponentLocation = Location;
		ComponentLocation.Z += LegSize;
		InMesh->SetWorldLocation(ComponentLocation);
	}
}

void IKneadingInterface::InitKneadingLocation(const FVector& InLocation)
{
	Location = InLocation;
}
