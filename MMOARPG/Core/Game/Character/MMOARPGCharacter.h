// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "MMOARPGType.h"
#include "Core/MMOARPGCharacterBase.h"
#include "MMOARPGCharacter.generated.h"

UCLASS(config=Game)
class AMMOARPGCharacter : public AMMOARPGCharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMMOARPGCharacter();



	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
#if 0
	/** Resets HMD orientation in VR. */
	void OnResetVR();
#endif


	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector InLocation);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector InLocation);

	void SwitchFight();

	UFUNCTION(Server,Reliable)
	void ActionSwitch();
	//服务器广播
	UFUNCTION(NetMulticast,Reliable)
	void MulticastActionSwitch();

//加减速
	UFUNCTION(Server, Reliable)
	void Fast();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastFast();

	UFUNCTION(Server, Reliable)
		void SlowDown();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSlowDown();

	UFUNCTION(Server, Reliable)
		void SlowDownReleased();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastSlowDownReleased();

	UFUNCTION(Server, Reliable)
	void FastReleased();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFastReleased();

	//左右回旋
	UFUNCTION(Server, Reliable)
	void DodgeLeft();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDodgeLeft();
	UFUNCTION(Server, Reliable)
	void DodgeRight();
	UFUNCTION(NetMulticast, Reliable)
		void MulticastDodgeRight();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void OnRep_ActionStateChanged();
	void FightChanged();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

