// Copyright KhalorlStudios


#include "Player/AuraPlayerState.h"
#include "AbilitySystem\AbilitySystemComponentBase.h"
#include "AbilitySystem\AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	this->NetUpdateFrequency = 100.0;
	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	this->AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}
