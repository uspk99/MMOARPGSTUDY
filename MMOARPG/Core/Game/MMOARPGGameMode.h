// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Channel/SimpleChannel.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "MMOARPGGameMode.generated.h"

UCLASS(minimalapi)
class AMMOARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMMOARPGGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void BindClientRcv();

	UFUNCTION()
	void LinkServerInfo(ESimpleNetErrorType Intype, const FString& InMsg);
	void LinkServer();
	void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);

protected:
	//½Ó¿Ú
	virtual void PostLogin(APlayerController* NewPlayer) override;
private:
	FDelegateHandle RecvDelegate;

public:
	void LoginCharacterUpdateKneadingRequest(int32 InUserID);

	void GetCharacterDataRequests(int32 InUserID,int32 InCharacterID);
};



