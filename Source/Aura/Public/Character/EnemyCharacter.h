// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/IHighlightable.h"
#include "UI/Controller/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyCharacter.generated.h"


class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;
/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public ACharacterBase, public IIHighlightable
{
	GENERATED_BODY()
	
	// Lesson learned, when creating interfaces don't add the I myself

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeDefaultAttributes() const override;

	AEnemyCharacter();

	virtual void Highlight() override;
	virtual void UnHighlight() override;

	virtual void InitAbilityActorInfo() override;
	
	// Do not need to replicate as only the server will use the value
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Elementalist;

	virtual int32 GetLevel() override;

	// Using the delegate defined in OverlayWidgetController.h
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DeathLifeSpan = 5.0;

	virtual void Die() override;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	virtual void PossessedBy(AController* NewController) override;
};
