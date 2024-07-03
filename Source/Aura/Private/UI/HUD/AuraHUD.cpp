// Copyright KhalorlStudios


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/BaseUserWidget.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "UI/Controller/AttributeMenuWidgetController.h"


// Connects the overlay Widget and controller and connects them
// Called in AuraCharacter::InitAbilityActorInfo
void AAuraHUD::InitOverlay(const FWidgetControllerParams& params)
{
	checkf(this->OverlayWidgetClass, TEXT("Widget Class not set"));
	checkf(this->OverlayWidgetControllerClass, TEXT("Widget Controller not set"));

	UUserWidget* widget = CreateWidget<UUserWidget>(this->GetWorld(), this->OverlayWidgetClass);
	this->OverlayWidget = Cast<UBaseUserWidget>(widget);

	UOverlayWidgetController* overlayWidgetController = this->GetOverlayWidgetController(params);

	this->OverlayWidget->SetWidgetController(overlayWidgetController);
	overlayWidgetController->BroadcastInitialValues();
	widget->AddToViewport();
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& params)
{
	if (this->AttributeMenuWidgetController)
	{
		return this->AttributeMenuWidgetController;
	}
	else
	{
		this->AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, this->AttributeMenuWidgetControllerClass);
		this->AttributeMenuWidgetController->SetWidgetControllerParams(params);
		this->AttributeMenuWidgetController->BindCallbacksToDependencies();
		return this->AttributeMenuWidgetController;
	}
}

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& params)
{
	if (this->OverlayWidgetController) 
	{
		return this->OverlayWidgetController;
	}
	else
	{
		this->OverlayWidgetController = NewObject<UOverlayWidgetController>(this, this->OverlayWidgetControllerClass);
		this->OverlayWidgetController->SetWidgetControllerParams(params);
		this->OverlayWidgetController->BindCallbacksToDependencies();
		return this->OverlayWidgetController;
	}
}


