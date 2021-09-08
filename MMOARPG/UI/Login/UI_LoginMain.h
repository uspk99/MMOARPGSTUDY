// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleNetChannelType.h"
#include "MMOARPGType.h"
#include "../Core/UI_MainBase.h"
#include "UI_LoginMain.generated.h"

class FSimpleChannel;
class UUI_Login;
class UUI_Print;
class UUI_Register;
/**
 * 
 */
UCLASS()
class MMOARPG_API UUI_LoginMain : public UUI_MainBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta =(BindWidget))
	UUserWidget* UI_LinkWidget;
	
	UPROPERTY(meta = (BindWidget))
	UUI_Login* UI_Login;

	UPROPERTY(meta = (BindWidget))
	UUI_Register* UI_Register;
public:
	virtual void NativeConstruct();//¹¹Ôì

	virtual void NativeDestruct();//Îö¹¹

	void SignIn(FString&, FString&);

	void Register();
	void Register(FString InRegisterInfo);

protected:

	virtual void RecvProtocol(uint32 ProtocolNumber, FSimpleChannel* Channel);


		virtual void LinkServerInfo(ESimpleNetErrorType Intype,const FString& InMsg);

};
