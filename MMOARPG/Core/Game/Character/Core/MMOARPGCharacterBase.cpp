// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGCharacterBase.h"
#include "../../MMOARPGGameState.h"
#include "Net/UnrealNetwork.h"
#include "../../Animation/Instance/Core/MMOARPGAnimInstanceBase.h"
#include "../../../../MMOARPGGameInstance.h"

// Sets default values
AMMOARPGCharacterBase::AMMOARPGCharacterBase()
	:bFight(false),ID(INDEX_NONE)
	,UserID(1)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMMOARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld())
	{
		if (AMMOARPGGameState* InGameState = GetWorld()->GetGameState<AMMOARPGGameState>())
		{
			if (FCharacterAnimTable* InAnimTable = InGameState->GetCharacterAnimTable(GetID()))
			{
				AnimTable = InAnimTable;
			}
		}
		if (!GetWorld()->IsServer())
		{
			if (GetMesh())
			{
				if (UMMOARPGAnimInstanceBase* InstanceBase
					= Cast<UMMOARPGAnimInstanceBase>(GetMesh()->GetAnimInstance()))
				{
					InstanceBase->InitAnimInstance(this);
				} 
			}

		}
	}
}

// Called every frame
void AMMOARPGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMMOARPGCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMMOARPGCharacterBase::OnRep_FightChanged()
{

}

void AMMOARPGCharacterBase::SwitchFightOnServer_Implementation(bool bNewFight)
{
	bFight = bNewFight;
}

void AMMOARPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//更新变量   只更新模拟玩家
	DOREPLIFETIME_CONDITION(AMMOARPGCharacterBase, bFight, COND_SimulatedOnly);
}

void AMMOARPGCharacterBase::AnimSignal(int32 InSignal)
{
	K2_AnimSignal(InSignal);
}