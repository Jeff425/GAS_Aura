// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraController.generated.h"

class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IIHighlightable;
class UAbilitySystemComponentBase;
class USplineComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	AAuraController();	
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	TScriptInterface<IIHighlightable> LastActor;
	TScriptInterface<IIHighlightable> ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponentBase> AuraAbilitySystemComponent;

	UAbilitySystemComponentBase* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0;
	float ShortPressThreshold = 0.5;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	FHitResult CursorHit;
};
