// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/MotionComponent.h"
#include "../../MMOARPGGameType.h"
#include "ClimbingComponent.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API UClimbingComponent : public UMotionComponent
{
	GENERATED_BODY()

public:
	UClimbingComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	EClimbingState ClimbingState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		bool bJumpToClimbing;

	FResultBool bJump;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ResetJump();

	void ClimbingForwardAxis(float InAxisValue);

	void ClimbingRightAxis(float InAxisValue);

	UFUNCTION(BlueprintCallable,Category="Climbing")
	void PhysClimbing(float deltaTime, int32 Iterations);

	void ReleaseClimbing();
	void Climbing();
private:
	void TraceClimbingState(float DeltaTime);

	void SetClimbingState(EMovementMode InMode, ECharacterActionState InCharacterActionState,bool bOrientRotation);




};
