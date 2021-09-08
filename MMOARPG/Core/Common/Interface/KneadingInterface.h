// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MMOARPGType.h"
#include "KneadingInterface.generated.h"

// This class does not need to be modified.
//Blueprintable,BlueprintType 接口以参数形式在蓝图暴露
UINTERFACE(MinimalAPI,Blueprintable,BlueprintType,meta=(CannotImplementInterfaceInBlueprint))
class UKneadingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MMOARPG_API IKneadingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	IKneadingInterface();

	//设置捏脸参数
	virtual void UpdateKneadingBody() = 0;
	virtual void UpdateKneadingBody(const FMMOARPGCharacterAppearance& InCA) = 0;
	//

	UFUNCTION(BlueprintCallable,Category="Kneading")
		virtual void SetLegSize(float InLegSize);
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual void SetWaistSize(float InWaistSize);
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual void SetArmSize(float InArmSize);
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual void SetHeadtSize(float InHeadSize);
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual void SetChestSize(float InChestSize);
	
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual float GetLegSize();
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual float GetWaistSize();
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual float GetArmSize();
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual float GetHeadSize();
	UFUNCTION(BlueprintCallable, Category = "Kneading")
		virtual float GetChestSize();
	
	//设置character骨骼体，在蓝图调用需要virtual
	void SetMeshPostion(USceneComponent* InMesh);
	//获取角色位置
	void InitKneadingLocation(const FVector& InLocation);
protected:
	float LegSize;
	float WaistSize;
	float ArmSize;
	float HeadSize;
	float ChestSize;

	FVector Location;
};
