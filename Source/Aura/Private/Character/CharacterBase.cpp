// Copyright KhalorlStudios


#include "Character/CharacterBase.h"


ACharacterBase::ACharacterBase()
{
	this->PrimaryActorTick.bCanEverTick = false;
	this->Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	this->Weapon->SetupAttachment(GetMesh(), "WeaponHandSocket");
	this->Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return this->AbilitySystemComponent;
}

void ACharacterBase::InitAbilityActorInfo() {}

