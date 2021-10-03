
#include "MMOARPGBoxHit.h"
#include "Components/BoxComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags.h"
#include "../../Character/Core/MMOARPGCharacterBase.h"

AMMOARPGBoxHit::AMMOARPGBoxHit(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void AMMOARPGBoxHit::HandleDamage(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	Super::HandleDamage(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult);
	//获取释放者,判断重叠对象不是自身
	if (GetInstigator()!=OtherActor)
	{
		//判断目标类型
		if (OtherActor->IsA(AMMOARPGCharacterBase::StaticClass()))
		{
			//设置时间数据结构体
			FGameplayEventData EventData;
			EventData.Instigator=GetInstigator();
			EventData.Target=OtherActor;

			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
				GetInstigator(),
				FGameplayTag::RequestGameplayTag(TEXT("Player.Attack.ComboLinkage")),
				EventData);
		}

	}
	
}

void AMMOARPGBoxHit::BeginPlay()
{
	Super::BeginPlay();
}

void AMMOARPGBoxHit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
