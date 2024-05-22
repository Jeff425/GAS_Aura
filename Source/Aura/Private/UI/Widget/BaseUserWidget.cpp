// Copyright KhalorlStudios


#include "UI/Widget/BaseUserWidget.h"

void UBaseUserWidget::SetWidgetController(UObject* InWidgetController)
{
	this->WidgetController = InWidgetController;
	this->WidgetControllerSet();
}
