// Copyright KhalorlStudios


#include "UI/Controller/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <AbilitySystem/AbilitySystemComponentBase.h>

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* auraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	OnHealthChange.Broadcast(auraAttributeSet->GetHealth());
	OnMaxHealthChange.Broadcast(auraAttributeSet->GetMaxHealth());
	OnManaChange.Broadcast(auraAttributeSet->GetMana());
	OnMaxManaChange.Broadcast(auraAttributeSet->GetMaxMana());
}

// Called in AuraHUD::GetOverlayWidgetController
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* auraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAbilitySystemComponentBase>(this->AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& tag : AssetTags)
			{
				// X.Y.Z can match X.Y but X.Y does not match X.Y.Z
				// In this case Message.HealthPotion would match Message
				if (tag.MatchesTag(FGameplayTag::RequestGameplayTag("Message")))
				{
					FUIWidgetRow* row = this->GetDataTableRowByTag<FUIWidgetRow>(this->MessageWidgetDataTable, tag);
					this->OnMessageWidgetRow.Broadcast(*row);
				}
			}
		}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChange.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChange.Broadcast(Data.NewValue);
}
