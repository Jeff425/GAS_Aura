// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

// This class does not need to be modified.
// Added BlueprintType so objects can be casted to interface in Blueprints
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual int32 GetLevel();

	// This should return the location on the character where projectiles spawn from
	// CharacterBase.h implements this function
	virtual FVector GetCombatSocketLocation();

	// By making NativeEvent, it will create a virtual function GetHitReactMontage_Implementable for C++, but blueprints can also implement it
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	// BlueprintImplementableEvents do not need to be virtual
	// Is called when casting an ability at a target, allows for blueprints to pivot to the location
	// This example uses Motion Warping to pivot to the target
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	virtual void Die() = 0;
};
