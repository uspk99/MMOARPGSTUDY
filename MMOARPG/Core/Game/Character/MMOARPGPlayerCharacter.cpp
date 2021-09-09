// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGPlayerCharacter.h"

void AMMOARPGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitKneadingLocation(GetMesh()->GetComponentLocation());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA)
{
	SetLegSize(InCA.LegSize);
	SetWaistSize(InCA.WaistSize);
	SetArmSize(InCA.ArmSize);
	SetHeadtSize(InCA.HeadSize);
	SetChestSize(InCA.ChestSize);

	//需要Mesh与骨骼一致变化
	SetMeshPostion(GetMesh());
}

void AMMOARPGPlayerCharacter::UpdateKneadingBody()
{

}
