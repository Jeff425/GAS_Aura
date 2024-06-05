// Copyright KhalorlStudios


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <Interaction/CombatInterface.h>

UMMC_MaxHealth::UMMC_MaxHealth()
{
	this->VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	this->VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	this->VigorDef.bSnapshot = false;

	this->RelevantAttributesToCapture.Add(this->VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evalParams;
	evalParams.SourceTags = sourceTags;
	evalParams.TargetTags = targetTags;

	float vigor = 0.0;
	this->GetCapturedAttributeMagnitude(this->VigorDef, Spec, evalParams, vigor);
	vigor = FMath::Max(vigor, 0.f);

	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 level = combatInterface->GetLevel();


	return 80.f + (2.5f * vigor) + (10.0f * level);
}
