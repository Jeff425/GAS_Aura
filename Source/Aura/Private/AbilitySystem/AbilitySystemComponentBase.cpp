// Copyright KhalorlStudios


#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

void UAbilitySystemComponentBase::AbilityActorInfoSet()
{
	this->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAbilitySystemComponentBase::ClientEffectApplied);
}

void UAbilitySystemComponentBase::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);
	this->EffectAssetTags.Broadcast(tagContainer);
}

void UAbilitySystemComponentBase::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> abilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		if (const UAuraGameplayAbility* auraAbiliy = Cast<UAuraGameplayAbility>(abilitySpec.Ability))
		{
			abilitySpec.DynamicAbilityTags.AddTag(auraAbiliy->StartupInputTag);
			this->GiveAbility(abilitySpec);
		}
		//this->GiveAbilityAndActivateOnce(abilitySpec);
	}
}

void UAbilitySystemComponentBase::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& abilitySpec : this->GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			this->AbilitySpecInputPressed(abilitySpec);
			if (!abilitySpec.IsActive())
			{
				this->TryActivateAbility(abilitySpec.Handle);
			}
		}
	}
}

void UAbilitySystemComponentBase::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& abilitySpec : this->GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			this->AbilitySpecInputReleased(abilitySpec);
		}
	}
}
