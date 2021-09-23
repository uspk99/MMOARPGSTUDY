#include "MMOARPGAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UMMOARPGAbilitySystemComponent::UMMOARPGAbilitySystemComponent()
{

}

UMMOARPGAbilitySystemComponent* UMMOARPGAbilitySystemComponent::GetAbilitySystemComponentFromActor(
	const AActor* Actor, bool LookForComponent/* = false*/)
{
	return Cast<UMMOARPGAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}