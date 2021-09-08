// Fill out your copyright notice in the Description page of Project Settings.


#include "HallHUD.h"
#include "../../UI/Hall/UI_HallMain.h"

AHallHUD::AHallHUD()
{
	static ConstructorHelpers::FClassFinder<UUI_HallMain> HallMain_BPClass(
		TEXT("/Game/UI/Hall/UI_HallMain_BP"));//��֤content·��
	HallMainClass = HallMain_BPClass.Class;//��ÿؼ�
}


void AHallHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HallMainClass)
	{
		HallMain = CreateWidget<UUI_HallMain>(GetWorld(), HallMainClass);
		HallMain ->AddToViewport();//��д���֣�Ĭ����������ײ�
	}
}

