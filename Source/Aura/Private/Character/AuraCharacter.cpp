// Copyright KhalorlStudios


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/AuraController.h"
#include "UI/Controller/BaseWidgetController.h"
#include "UI/HUD/AuraHUD.h"


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
	// Player State exists for all characters on all clients
	AAuraPlayerState* auraPlayerState = this->GetPlayerState<AAuraPlayerState>();
	check(auraPlayerState);
	auraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(auraPlayerState, this);
	this->AbilitySystemComponent = auraPlayerState->GetAbilitySystemComponent();
	this->AttributeSet = auraPlayerState->AttributeSet;

	// Controller only exists for owning clients
	if (AAuraController* auraController = Cast<AAuraController>(this->GetController()))
	{
		// Assert check might make more sense, since we got the controller for aura earlier
		if (AAuraHUD* auraHUD = Cast<AAuraHUD>(auraController->GetHUD()))
		{
			auraHUD->InitOverlay(FWidgetControllerParams(auraController, auraPlayerState, this->AbilitySystemComponent, this->AttributeSet));
		}
	}

}
