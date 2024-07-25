// Copyright KhalorlStudios


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include <Interaction/CombatInterface.h>

// Private struct that is only used in this cpp file
struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Crit);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResistance);

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToCaptureDef;
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	AuraDamageStatics()
	{
		const FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Crit, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CritResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics dStatics;
	// Everytime this function is called, it will attempt to fill out the TagToCaptureDef
	if (dStatics.TagToCaptureDef.Num() < 1)
	{
		const FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();
		// Only fill if the tags have been set
		if (gameplayTags.Attributes_Secondary_Armor.IsValid())
		{
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_Armor, dStatics.ArmorDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_ArmorPenetration, dStatics.ArmorPenetrationDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_Crit, dStatics.CritDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_CritDamage, dStatics.CritDamageDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_CritResistance, dStatics.CritResistanceDef);

			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_FireResistance, dStatics.FireResistanceDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_LightningResistance, dStatics.LightningResistanceDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_ArcaneResistance, dStatics.ArcaneResistanceDef);
			dStatics.TagToCaptureDef.Add(gameplayTags.Attributes_Secondary_PhysicalResistance, dStatics.PhysicalResistanceDef);
		}
	}
	return dStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	this->RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritDamageDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().CritResistanceDef);

	this->RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	this->RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
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
	FGameplayEffectContextHandle contextHandle = spec.GetContext();

	const FGameplayTagContainer* sourceTags = spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evalParams;
	evalParams.SourceTags = sourceTags;
	evalParams.TargetTags = targetTags;

	// Get Damage Set by Caller Magnitude
	float damage = 0.0f; //spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	// Fetch all damage tags, besides the parent tag
	for (const auto& tagPair : FAuraGameplayTags::Get().DamageTypesAndResistances)
	{
		checkf(DamageStatics().TagToCaptureDef.Contains(tagPair.Value), TEXT("TagsToCaptureDef does not contain Tag: [%s] in ExecCalc_Damage. TagToCaptureDef has %d elements. First Element is [%s]"), *tagPair.Value.ToString(), DamageStatics().TagToCaptureDef.Num(), *DamageStatics().TagToCaptureDef.begin().Key().ToString());
		const FGameplayEffectAttributeCaptureDefinition captureDef = DamageStatics().TagToCaptureDef[tagPair.Value];
		float tagResistance = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(captureDef, evalParams, tagResistance);
		tagResistance = FMath::Clamp(tagResistance, 0.0f, 100.0f);
		float tagDamage = spec.GetSetByCallerMagnitude(tagPair.Key);

		damage += tagDamage * (1.0f - (tagResistance * 0.01));
	}



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
		UAuraAbilitySystemLibrary::SetCriticalHit(contextHandle, true);
	}
	else
	{
		UAuraAbilitySystemLibrary::SetCriticalHit(contextHandle, false);
	}

	const FGameplayModifierEvaluatedData evalData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, damage);
	OutExecutionOutput.AddOutputModifier(evalData);
}
