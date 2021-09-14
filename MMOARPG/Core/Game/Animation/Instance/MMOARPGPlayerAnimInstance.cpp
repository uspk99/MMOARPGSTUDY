// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGPlayerAnimInstance.h"
#include "../../../Common/Interface/KneadingInterface.h"


void UMMOARPGPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IKneadingInterface* InCharacterBase = Cast<IKneadingInterface>(TryGetPawnOwner()))
	{
		LegSize = InCharacterBase->GetLegSize();
		WaistSize = InCharacterBase->GetWaistSize();
		ArmSize = InCharacterBase->GetArmSize();
		HeadSize = InCharacterBase->GetHeadSize();
		ChestSize = InCharacterBase->GetChestSize();
	}
}
