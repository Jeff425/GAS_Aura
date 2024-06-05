// Copyright KhalorlStudios


#include "Character/EnemyCharacter.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"

// I did this in BP_EnemyBase
// But this->GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); Under the constructor can set the collision channel in code

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->InitAbilityActorInfo();
}

AEnemyCharacter::AEnemyCharacter()
{
	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	this->AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AEnemyCharacter::Highlight()
{

	this->GetMesh()->SetRenderCustomDepth(true);
	this->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if (this->Weapon) {
		this->Weapon->SetRenderCustomDepth(true);
		this->Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AEnemyCharacter::UnHighlight()
{
	this->GetMesh()->SetRenderCustomDepth(false);
	if (this->Weapon) {
		this->Weapon->SetRenderCustomDepth(false);
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	this->AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Both this and Aura character are calling this. Feels like there's a better way...
	Cast<UAbilitySystemComponentBase>(this->AbilitySystemComponent)->AbilityActorInfoSet();
}

int32 AEnemyCharacter::GetLevel()
{
	return this->Level;
}
