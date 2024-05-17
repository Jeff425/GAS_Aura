// Copyright KhalorlStudios


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"

AAuraCharacter::AAuraCharacter()
{
	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->RotationRate = FRotator(0.0, 400.0, 0.0);
	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->bSnapToPlaneAtStart = true;

	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	this->InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	this->InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* auraPlayerState = this->GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	auraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(auraPlayerState, this);
	this->AbilitySystemComponent = auraPlayerState->GetAbilitySystemComponent();
	this->AttributeSet = auraPlayerState->AttributeSet;
}
