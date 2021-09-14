// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMOARPG/DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "MMOARPGCharacterBase.generated.h"

UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : public ACharacter,public ISimpleCombatInterface
{
	GENERATED_BODY()
		
	friend class AMMOARPGGameMode;
public:
	// Sets default values for this character's properties
	AMMOARPGCharacterBase();

	virtual void AnimSignal(int32 InSignal);
public:
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,DisplayName="AnimSignal",Category="AnimEcent")
	void K2_AnimSignal(int32 InSignal);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE bool IsFight() { return bFight;}

	FORCEINLINE FCharacterAnimTable *GetAnimTable() { return AnimTable; }
	FORCEINLINE int32 GetID() { return ID; }
	FORCEINLINE int32 GetUserID() { return UserID; }
protected:
	//服务器，同步
	UFUNCTION(Server,Reliable)
	void SwitchFightOnServer(bool bNewFight);
protected:
	//更新
	UPROPERTY(ReplicatedUsing = OnRep_FightChanged)
	bool bFight;

	UFUNCTION()
	virtual void OnRep_FightChanged();

	UPROPERTY(EditDefaultsOnly,Category="Character")
	int32 ID;

	int32 UserID;
protected:
	FCharacterAnimTable* AnimTable;
};
