// Copyright KhalorlStudios


#include "Character/CharacterBase.h"
#include <AbilitySystem/AbilitySystemComponentBase.h>


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

FVector ACharacterBase::GetCombatSocketLocation()
{
	return this->Weapon->GetSocketLocation(WeaponTipSocketName);
}

void ACharacterBase::InitAbilityActorInfo() {}

void ACharacterBase::InitializeDefaultAttributes() const
{
	this->ApplyEffectToSelf(this->DefaultPrimaryAttributes, 1.0);
	this->ApplyEffectToSelf(this->DefaultSecondaryAttributes, 1.0);
	this->ApplyEffectToSelf(this->DefaultVitalAttributes, 1.0);
}

void ACharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(this->GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle contextHandle = this->GetAbilitySystemComponent()->MakeEffectContext();
	contextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle specHandle = this->GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, contextHandle);
	// Applys effect spec to target, which is itself LOL
	this->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), this->GetAbilitySystemComponent());
}

void ACharacterBase::AddCharacterAbilities()
{
	// Server only
	if (!this->HasAuthority())
	{
		return;
	}

	UAbilitySystemComponentBase* auraASC = CastChecked<UAbilitySystemComponentBase>(this->AbilitySystemComponent);
	auraASC->AddCharacterAbilities(this->StartupAbilities);
}

