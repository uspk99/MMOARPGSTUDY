// Fill out your copyright notice in the Description page of Project Settings.


#include "MMOARPGCharacterBase.h"
#include "../../MMOARPGGameState.h"
#include "Net/UnrealNetwork.h"
#include "../../Animation/Instance/Core/MMOARPGAnimInstanceBase.h"
#include "../../../../MMOARPGGameInstance.h"
#include "MMOARPG/Core/Component/FlyComponent.h"
#include "../../../../Core/Component/SwimmingComponent.h"
#include "../../../../Core/Component/ClimbingComponent.h"
#include "../../../../Core/Component/FightComponent.h"
#include "../../Abilities/MMOARPGAbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "../../../../Core/Component/GameFramework/MMOARPGMovementComponent.h"


void AMMOARPGCharacterBase::NormalAttack(const FName& InKey)
{
	GetFightComponent()->NormalAttack(InKey);
}

void AMMOARPGCharacterBase::ComboAttack(const FName& InKey)
{
	NormalAttack(InKey);
}

FSimpleComboCheck* AMMOARPGCharacterBase::GetSimpleComboInfo()
{
	return GetFightComponent()->GetSimpleComboInfo();
}

// Sets default values
AMMOARPGCharacterBase::AMMOARPGCharacterBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMMOARPGMovementComponent>(ACharacter::CharacterMovementComponentName))
	,ActionState(ECharacterActionState::NORMAL_STATE)
	,LastActionState(ECharacterActionState::NORMAL_STATE)
	,ID(INDEX_NONE)
	,UserID(1)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlyComponent = CreateDefaultSubobject<UFlyComponent>(TEXT("FlyComponent"));
	SwimmingComponent = CreateDefaultSubobject<USwimmingComponent>(TEXT("SwimmingComponent"));
	ClimbingComponent = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));
	//FlyComponent->SetupAttachment(RootComponent);
	FightComponent = CreateDefaultSubobject<UFightComponent >(TEXT("FightComponent "));
	FightComponent->SetIsReplicated(true);
	AbilitySystemComponent = CreateDefaultSubobject<UMMOARPGAbilitySystemComponent >(TEXT("AbilitySystemComponent "));
	//爬行可同步.
	ClimbingComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AMMOARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

void AMMOARPGCharacterBase::ResetActionState(ECharacterActionState InActionState)
{
	//客户端
	if (ActionState== InActionState)
	{
		ActionState = ECharacterActionState::NORMAL_STATE;
	}
	else
	{
		ActionState = InActionState;
	}
}


void AMMOARPGCharacterBase::SwitchActionStateOnServer_Implementation(ECharacterActionState InActionState)
{
	//bFight = bNewFight;
	ActionState = InActionState;
	LastActionState = ActionState;


}

void AMMOARPGCharacterBase::OnRep_ActionStateChanged()
{

}

void AMMOARPGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//更新变量   只更新模拟玩家
	DOREPLIFETIME_CONDITION(AMMOARPGCharacterBase, ActionState, COND_SimulatedOnly);
}
void AMMOARPGCharacterBase::AnimSignal(int32 InSignal)
{
	K2_AnimSignal(InSignal);
}

void AMMOARPGCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (LastActionState==ECharacterActionState::CLIMB_STATE)
	{
		//StopAnimMontage();
	}

}
