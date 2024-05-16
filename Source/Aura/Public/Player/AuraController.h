// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraController.generated.h"

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
	TObjectPtr<class UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<class UInputAction> MoveAction;

	void Move(const struct FInputActionValue& InputActionValue);
	void CursorTrace();

	TScriptInterface<class IIHighlightable> LastActor;
	TScriptInterface<IIHighlightable> ThisActor;
};
