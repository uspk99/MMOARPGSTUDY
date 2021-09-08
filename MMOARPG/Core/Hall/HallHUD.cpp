// Fill out your copyright notice in the Description page of Project Settings.


#include "HallHUD.h"
#include "../../UI/Hall/UI_HallMain.h"

AHallHUD::AHallHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_HallMain> HallMain_BPClass(
		TEXT("/Game/UI/Hall/UI_HallMain_BP"));//保证content路径
	HallMainClass = HallMain_BPClass.Class;//获得控件
}


void AHallHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HallMainClass)
	{
		HallMain = CreateWidget<UUI_HallMain>(GetWorld(), HallMainClass);
		HallMain ->AddToViewport();//不写数字，默认生成在最底层
	}
}

