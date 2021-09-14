// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MMOARPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMMOARPGPlayerController();

	UFUNCTION(Server, Reliable)
	void ReplaceCharacter(int32 InCharacterID);
	
};
