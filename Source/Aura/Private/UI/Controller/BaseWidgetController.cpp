// Copyright KhalorlStudios


#include "UI/Controller/BaseWidgetController.h"

void UBaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& params)
{
	this->PlayerController = params.PlayerController;
	this->PlayerState = params.PlayerState;
	this->AbilitySystemComponent = params.AbilitySystemComponent;
	this->AttributeSet = params.AttributeSet;
}
