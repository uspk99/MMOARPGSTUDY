// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/SimpleBrowse.h"
#include "Core/SimpleZoom.h"
#include "Core/SImplePanelMove.h"
#include "HallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AHallPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		AHallPlayerController();

		//执行 停止 旋转
		void ExecutionRotateCharacter();
		void StopRotateCharacter();

		void ResetTarget(AActor* InTarget);

		//缩放
		void Zoom(float InDeltaTime);

		//上下移动
		void BeginMove();
		void EndMove();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//released 绑定输入
	virtual void SetupInputComponent();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	SimpleActorAction::FSimpleBrowse SimpleBrowse;
	SimpleActorAction::FSimpleZoom SimpleZoom;
	SimpleActorAction::FSimplePanelMove SimplePanelMove;
};
