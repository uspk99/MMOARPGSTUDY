// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGPlayerController.h"
#include "Character/MMOARPGCharacter.h"
#include "MMOARPGGameState.h"
#include "Character/Core/MMOARPGCharacterBase.h"
#include "Character/MMOARPGPlayerCharacter.h"
#include "MMOARPGPlayerState.h"

AMMOARPGPlayerController::AMMOARPGPlayerController()
{
	bShowMouseCursor = true;
}

void AMMOARPGPlayerController::ReplaceCharacter_Implementation(int32 InCharacterID)
{
	if (AMMOARPGCharacterBase* MMOARPGBase = GetPawn<AMMOARPGCharacterBase>())
	{
		if (MMOARPGBase->GetID() == InCharacterID)
		{
			//IDһ���Ͳ��øĽ�ɫ
			return;
		}
	}
	if (AMMOARPGGameState* InState = GetWorld()->GetGameState<AMMOARPGGameState>())
	{
		if (FCharacterStyleTable* InStyleTable = InState->GetCharacterStyleTable(InCharacterID))
		{
			if (AMMOARPGCharacter* InNewCharacter =GetWorld()->
				SpawnActor<AMMOARPGCharacter>
				(InStyleTable->MMOARPGCharacterClass,
					GetPawn()->GetActorLocation(),
					GetPawn()->GetActorRotation()))
			{
				if (AMMOARPGPlayerState* InPlayerState = GetPlayerState<AMMOARPGPlayerState>())
				{
					//�ж���Ҫ���
					if (AMMOARPGPlayerCharacter* InPlayerCharacter = Cast<AMMOARPGPlayerCharacter>(InNewCharacter))
					{
						InPlayerCharacter->UpdateKneadingBody(InPlayerState->GetCA());
						InPlayerCharacter->CallUpdateKneadingBodyOnClient(InPlayerState->GetCA());
					}
				}


				APawn* InPawn = GetPawn();
				//�Ľ�ɫ
				OnPossess(InNewCharacter);
				//Destroy(true);ǿ������ɾ��
				InPawn->Destroy(true);
			}
		}
	}

}
