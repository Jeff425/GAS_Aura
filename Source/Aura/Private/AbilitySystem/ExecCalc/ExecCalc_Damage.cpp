// Copyright KhalorlStudios


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include <Interaction/CombatInterface.h>

// Private struct that is only used in this cpp file
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Crit);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResistance);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Crit, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics dStatics;
	return dStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	this->RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* sourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* targetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* sourceAvatar = sourceASC ? sourceASC->GetAvatarActor() : nullptr;
	AActor* targetAvatar = targetASC ? targetASC->GetAvatarActor() : nullptr;
	ICombatInterface* sourceCombat = Cast<ICombatInterface>(sourceAvatar);
	ICombatInterface* targetCombat = Cast<ICombatInterface>(targetAvatar);

	const FGameplayEffectSpec& spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evalParams;
	evalParams.SourceTags = sourceTags;
	evalParams.TargetTags = targetTags;

	// Get Damage Set by Caller Magnitude
	float damage = spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	// Get Blockchance on target, determine if block was successful
	// If block works, half the damage

	// I have decided not to use block chance in this game, but block chance would be added like below
	/*
	float blockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, evalParams, blockChance);
	blockChance = FMath::Max(blockChance, 0.0f);

	const bool bBlocked = FMath::RandRange(1, 100) < blockChance;
	if (bBlocked) damage *= 0.5;
	*/

	UCharacterClassInfo* classInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(sourceAvatar);
	FRealCurve* armorCurve = classInfo->DamageCalculationCoefficients->FindCurve(FName("Armor"), FString());
	FRealCurve* armorPenCurve = classInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	// EW FUCKING GROSS THIS IS A CONSTANT CURVE THOOO???
	// So as soon as a unit hits level 10 they get nerfed????????????????
	// BAD GAME DESIGN LMAO WTF
	const float armorCoeff = armorCurve->Eval(targetCombat->GetLevel());
	const float armorPenCoeff = armorPenCurve->Eval(sourceCombat->GetLevel());

	float armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, evalParams, armor);
	armor = FMath::Max(armor, 0.0f);
	
	float armorPen = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, evalParams, armorPen);
	armorPen = FMath::Max(armorPen, 0.0f);

	// ArmorPen ignores a percentage of the Target's Armor
	const float effectiveArmor = armor * ((100.0f - (armorPen * armorPenCoeff)) / 100.0f);
	damage *= (100.0f - effectiveArmor * armorCoeff) / 100.0f;
	damage = FMath::Max(damage, 0.0f);

	// Crit Doubles the damage and adds extra unblocked pure damage
	float baseCrit = 0.0f;
	float critRes = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDef, evalParams, baseCrit);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResistanceDef, evalParams, critRes);
	float critChance = FMath::Max(baseCrit - critRes, 0.0f);
	if (FMath::RandRange(1, 100) < critChance)
	{
		// Double damage
		damage *= 2.0;

		// Add bonus Damage
		float bonusDamage = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDamageDef, evalParams, bonusDamage);
		damage += bonusDamage;
	}

	const FGameplayModifierEvaluatedData evalData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evalData);
}
