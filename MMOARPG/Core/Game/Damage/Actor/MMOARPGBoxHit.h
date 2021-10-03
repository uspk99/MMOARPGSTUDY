// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hit/ComboSkillHitBox.h"
#include "MMOARPGBoxHit.generated.h"

//标签，成为变量，可继承为蓝图
UCLASS(BlueprintType, Blueprintable)
class AMMOARPGBoxHit : public AHitBoxCollision
{
	GENERATED_BODY()
public:
	AMMOARPGBoxHit(const FObjectInitializer& ObjectInitializer);

	virtual void HandleDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};