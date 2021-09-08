// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "LoginHUD.h"
#include "LoginPawn.h"
#include "LoginPlayerController.h"

ALoginGameMode::ALoginGameMode()
{
	HUDClass = ALoginHUD::StaticClass();//นุมช
	DefaultPawnClass = ALoginPawn::StaticClass();
	PlayerControllerClass = ALoginPlayerController::StaticClass();
}
