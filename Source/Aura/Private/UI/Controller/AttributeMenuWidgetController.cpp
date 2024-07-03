// Copyright KhalorlStudios


#include "UI/Controller/AttributeMenuWidgetController.h"
#include <AbilitySystem/AuraAttributeSet.h>
#include <AuraGameplayTags.h>

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	check(this->AttributeInfo);

	for (auto& pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo info = this->AttributeInfo->FindAttributeInfoForTag(pair.Key);
		info.AttributeValue = pair.Value.Execute().GetNumericValue(AS);
		this->AttributeInfoDelegate.Broadcast(info);
	}
}

// TODO: This is untested
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	check(this->AttributeInfo);
	for (auto& pair : AS->TagsToAttributes)
	{
		this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value.Execute()).AddLambda([this, pair, AS](const FOnAttributeChangeData& data)
			{
				FAuraAttributeInfo info = this->AttributeInfo->FindAttributeInfoForTag(pair.Key);
				info.AttributeValue = pair.Value.Execute().GetNumericValue(AS);
				this->AttributeInfoDelegate.Broadcast(info);
			}
		);
	}
}
