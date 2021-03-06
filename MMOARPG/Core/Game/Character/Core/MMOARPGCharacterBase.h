// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MMOARPG/DataTable/CharacterAnimTable.h"
#include "CombatInterface/SimpleCombatInterface.h"
#include "MMOARPG/MMOARPGGameType.h"
#include "UObject/ObjectPtr.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "SImpleComboType.h"
#include "../../Abilities/MMOARPGAttributeSet.h"
#include "MMOARPGType.h"
#include "MMOARPGCharacterBase.generated.h"


class UFlyComponent;
class USwimmingComponent;
class UClimbingComponent;
class UFightComponent;
class UMMOARPGAbilitySystemComponent;
UCLASS()
class MMOARPG_API AMMOARPGCharacterBase : public ACharacter,public ISimpleComboInterface,public IAbilitySystemInterface
{
	GENERATED_BODY()
		
	friend class AMMOARPGGameMode;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFlyComponent> FlyComponent;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<USwimmingComponent>SwimmingComponent;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UClimbingComponent>ClimbingComponent;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UFightComponent>FightComponent;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UMMOARPGAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Category = MMOARPGCharacterBase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMMOARPGAttributeSet> AttributeSet;

public:
	UFUNCTION(BlueprintCallable)
	void NormalAttack(const FName& InKey);

	virtual void ComboAttack(const FName& InKey);

	virtual FSimpleComboCheck* GetSimpleComboInfo();

public:
	// Sets default values for this character's properties

	AMMOARPGCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual void AnimSignal(int32 InSignal);

	virtual void ClimbingMontageChanged(EClimbingMontageState InJumpState){}

	virtual void Landed(const FHitResult& Hit);
public:
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,DisplayName="AnimSignal",Category="AnimEvent")
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

	FORCEINLINE ECharacterActionState GetActionState() { return ActionState;}

	FORCEINLINE FCharacterAnimTable *GetAnimTable() { return AnimTable; }
	FORCEINLINE int32 GetID() { return ID; }
	FORCEINLINE int32 GetUserID() { return UserID; }

	FORCEINLINE UFlyComponent* GetFlyComponent() { return FlyComponent; }
	FORCEINLINE USwimmingComponent* GetSwimmingComponent() { return SwimmingComponent; }
	FORCEINLINE UClimbingComponent* GetClimbingComponent() { return ClimbingComponent; }
	FORCEINLINE UFightComponent* GetFightComponent() { return FightComponent; }
	void SetUserID(int32 InUserID) { UserID = InUserID; };

	void ResetActionState(ECharacterActionState InActionState);

	virtual class UCameraComponent* GetFollowCamera() const { return NULL; }

	UFUNCTION(NetMulticast,Reliable)
	void UpdateCharacterAttribute(const FMMOARPGCharacterAttribute &CharacterAttribute);
protected:
	//????????????
	UFUNCTION(Server,Reliable)
	void SwitchActionStateOnServer(ECharacterActionState InActionState);
protected:
	////????
	//UPROPERTY(ReplicatedUsing = OnRep_FightChanged)
	//bool bFight;

	//????????
	UPROPERTY(ReplicatedUsing = OnRep_ActionStateChanged)
		ECharacterActionState ActionState;

	//??????????
	UPROPERTY()
	ECharacterActionState LastActionState;

	UFUNCTION()
	virtual void OnRep_ActionStateChanged();

	UPROPERTY(EditDefaultsOnly,Category="Character")
	int32 ID;

	int32 UserID;
protected:
	FCharacterAnimTable* AnimTable;
};
