// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
	
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FAuraInputAction& action : InputConfig->AbilityInputActions)
	{
		if (action.InputAction && action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				this->BindAction(action.InputAction, ETriggerEvent::Started, Object, PressedFunc, action.InputTag);
			}
			if (ReleasedFunc)
			{
				this->BindAction(action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, action.InputTag);
			}
			if (HeldFunc)
			{
				this->BindAction(action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, action.InputTag);
			}
			
		}
	}
}
