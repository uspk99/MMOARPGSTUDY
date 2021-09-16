// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "../../MMOARPGGameType.h"
#include "FlyComponent.generated.h"

class AMMOARPGCharacterBase;
class UCharacterMovementComponent;
class UCapsuleComponent;
class UCameraComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMOARPG_API UFlyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		FVector2D RotationRate;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
	//	bool bFastFly;

	FResultBool bFastFly;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimAttrubute")
		EDodgeFly DodgeFly;

		FResultBool bLand;
public:	
	// Sets default values for this component's properties
	UFlyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Print(float InTime, const FString& InString);

	void Reset();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FlyForwardAxis(float InAxisValue);
	void ResetFly();
	void ResetFastFly();

	void ResetDodgeFly(EDodgeFly InDodgeFly);

	UFUNCTION()
	void Landed(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
protected:
	UPROPERTY()
	TWeakObjectPtr<AMMOARPGCharacterBase> MMOARPGCharacterBase;
	UPROPERTY()
	TWeakObjectPtr<UCharacterMovementComponent>  CharacterMovementComponent;
	UPROPERTY()
	TWeakObjectPtr<UCapsuleComponent>  CapsuleComponent;
	UPROPERTY()
	TWeakObjectPtr<UCameraComponent>  CameraComponent;

	UPROPERTY()
	FRotator LastRotator;
protected:
	//UPROPERTY()
	//float DodgeTime;
};
