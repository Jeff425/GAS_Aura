// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

// This is set under Config/DefaultGame.ini

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};
