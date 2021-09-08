// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"

UWidgetAnimation* UUI_Base::GetNameWidgetAnimation(const FString& InWidgetAnimName)
{
	if (UWidgetBlueprintGeneratedClass* WidgetBuleprintGenerated =
		Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBuleprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate(
			[&](const UWidgetAnimation* OurAnimation)
			{
				return OurAnimation->GetFName().ToString() == (InWidgetAnimName + FString("_INST"));
			});//遍历查找数组中满足对应函数条件的元素
		return *MyTempAnimation;
	}
	return NULL;
}

void UUI_Base::PlayWidgetAnim(const FString& InWidgetAnimName)
{
	if ( UWidgetAnimation* MyTempAnimation = GetNameWidgetAnimation(InWidgetAnimName))
	{
		PlayAnimation(MyTempAnimation);
	}
}
