// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle ContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FEffectProperties BuildEffectProperties(const FGameplayEffectModCallbackData& Data) const;

	void ShowFloatingText(const FEffectProperties& Props, float Damage) const;
	
	TMap<FGameplayTag, FAttributeSignature> TagsToAttributes;

	// Add Replicated for the stat to be replicated. Add ReplicatedUsing for a callback function to trigger
	// There must be a function in order to call the macro GAMEPLAYATTRIBUTE_REPNOTIFY
	// VITALS
	// Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vitals")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldVal) const;

	// Max Health
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vitals")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldVal) const;

	// Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vitals")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldVal) const;

	// Max Mana
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vitals")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldVal) const;

	// PRIMARY
	// Strength
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldVal) const;

	// Intelligence
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldVal) const;

	// Resilience
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldVal) const;

	// Vigor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVal) const;

	// SECONDARY
	// Armor
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Secondary")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldVal) const;

	// Armor Penetration
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category = "Secondary")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldVal) const;

	// Crit
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Crit, Category = "Secondary")
	FGameplayAttributeData Crit;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Crit);

	UFUNCTION()
	void OnRep_Crit(const FGameplayAttributeData& OldVal) const;

	// Crit Damage
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritDamage, Category = "Secondary")
	FGameplayAttributeData CritDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritDamage);

	UFUNCTION()
	void OnRep_CritDamage(const FGameplayAttributeData& OldVal) const;

	// Crit Resistance
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CritResistance, Category = "Secondary")
	FGameplayAttributeData CritResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CritResistance);

	UFUNCTION()
	void OnRep_CritResistance(const FGameplayAttributeData& OldVal) const;

	// Health Regen
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Secondary")
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegen);

	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldVal) const;

	// Mana Regen
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Secondary")
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegen);

	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldVal) const;

	/*
	 * Meta Attributes
	 * These do not get replicated
	 */
	// Incoming Damage
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage);

	// Could we have multiple different types of "Incoming Damage" in order to do resistances/blocks/dodges depending on damage type?
};
