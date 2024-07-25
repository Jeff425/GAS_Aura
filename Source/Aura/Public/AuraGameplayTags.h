// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * Singleton containing native Gameplay Tags
 */

#define DAMAGE_AND_RESISTANCE_DEC(DamageType) \
FGameplayTag Damage_##DamageType; \
FGameplayTag Attributes_Secondary_ ## DamageType ## Resistance;

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_Crit;
	FGameplayTag Attributes_Secondary_CritDamage;
	FGameplayTag Attributes_Secondary_CritResistance;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	// Loose tag at this point?
	FGameplayTag Damage;

	TMap<FGameplayTag, FGameplayTag> DamageTypesAndResistances;
	DAMAGE_AND_RESISTANCE_DEC(Fire)
	DAMAGE_AND_RESISTANCE_DEC(Lightning)
	DAMAGE_AND_RESISTANCE_DEC(Arcane)
	DAMAGE_AND_RESISTANCE_DEC(Physical)

	FGameplayTag Effects_HitReact;

private:
	static FAuraGameplayTags GameplayTags;
};
