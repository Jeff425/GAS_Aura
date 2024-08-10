// Copyright KhalorlStudios


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle specHandle = this->MakeOutgoingGameplayEffectSpec(this->DamageEffectClass, 1.0f);
	for (TTuple<FGameplayTag, FScalableFloat> pair : this->DamageTypes)
	{
		float damageMag = pair.Value.GetValueAtLevel(this->GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, pair.Key, damageMag);
	}
	this->GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}
