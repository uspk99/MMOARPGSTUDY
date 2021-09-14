// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MMOARPGCharacter.h"
#include "../../Common/Interface/KneadingInterface.h"
#include "MMOARPGType.h"
#include "MMOARPGPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MMOARPG_API AMMOARPGPlayerCharacter : public AMMOARPGCharacter, public IKneadingInterface
{
	GENERATED_BODY()

public:
	//Robot
	UFUNCTION()
	void UpdateKneadingRequest();

	//�ͻ��˸��½�ɫ�ӿ�
	//��Ҫ��FMMOARPGCharacterAppearance��¶����ͼ
	UFUNCTION(Client, Reliable)
	void CallUpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA);

protected:
	//Reliable:TCP
	UFUNCTION(Server,Reliable)
	void CallServerUpdateKneading(int32 InUserID);

protected:
	virtual void BeginPlay() override;
public:
	//������������
	virtual void UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA);
	virtual void UpdateKneadingBody();
	
};
