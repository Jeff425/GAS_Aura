// Copyright KhalorlStudios

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual int32 GetLevel() override;
};
