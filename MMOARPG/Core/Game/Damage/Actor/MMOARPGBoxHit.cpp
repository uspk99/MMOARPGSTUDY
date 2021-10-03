
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
	//��ȡ�ͷ���,�ж��ص�����������
	if (GetInstigator()!=OtherActor)
	{
		//�ж�Ŀ������
		if (OtherActor->IsA(AMMOARPGCharacterBase::StaticClass()))
		{
			//����ʱ�����ݽṹ��
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
