// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMOARPGCharacter.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "MMOARPGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerCharacter : public AMMOARPGCharacter, public IKneadingInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	//…Ë÷√ƒÛ¡≥≤Œ ˝
	virtual void UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA);
	virtual void UpdateKneadingBody();
	
};
