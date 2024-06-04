// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Interaction/IHighlightable.h"
#include "EnemyCharacter.generated.h"

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
	AEnemyCharacter();

	virtual void Highlight() override;
	virtual void UnHighlight() override;

	virtual void InitAbilityActorInfo() override;
};
