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

	//客户端更新角色接口
	//需要将FMMOARPGCharacterAppearance暴露给蓝图
	UFUNCTION(Client, Reliable)
	void CallUpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA);

protected:
	//Reliable:TCP
	UFUNCTION(Server,Reliable)
	void CallServerUpdateKneading(int32 InUserID);

protected:
	virtual void BeginPlay() override;
public:
	//设置捏脸参数
	virtual void UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA);
	virtual void UpdateKneadingBody();
	
};
