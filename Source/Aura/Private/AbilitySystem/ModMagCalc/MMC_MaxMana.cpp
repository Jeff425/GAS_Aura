// Copyright KhalorlStudios


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <Interaction/CombatInterface.h>

UMMC_MaxMana::UMMC_MaxMana()
{
	this->IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	this->IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	this->IntDef.bSnapshot = false;

	this->RelevantAttributesToCapture.Add(this->IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evalParams;
	evalParams.SourceTags = sourceTags;
	evalParams.TargetTags = targetTags;

	float intelligence = 0.0;
	this->GetCapturedAttributeMagnitude(this->IntDef, Spec, evalParams, intelligence);
	intelligence = FMath::Max(intelligence, 0.f);

	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 level = combatInterface->GetLevel();


	return 50.f + (2.5f * intelligence) + (15.0f * level);
}
