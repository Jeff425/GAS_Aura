// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemComponentBase.generated.h"

// Create a delegate called FEffectAssetTags that expect a container as a param
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class AURA_API UAbilitySystemComponentBase : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
	
public:
	FEffectAssetTags EffectAssetTags;
	
	void AbilityActorInfoSet();
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
