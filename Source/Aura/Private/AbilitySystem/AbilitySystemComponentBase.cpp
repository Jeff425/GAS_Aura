// Copyright KhalorlStudios


#include "AbilitySystem/AbilitySystemComponentBase.h"

void UAbilitySystemComponentBase::AbilityActorInfoSet()
{
	this->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAbilitySystemComponentBase::EffectApplied);
}

void UAbilitySystemComponentBase::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);
	this->EffectAssetTags.Broadcast(tagContainer);
}
