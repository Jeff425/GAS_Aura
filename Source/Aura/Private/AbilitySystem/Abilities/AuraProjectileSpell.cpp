// Copyright KhalorlStudios


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = this->GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	FVector socketLocation = ICombatInterface::Execute_GetCombatSocketLocation(this->GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get().Montage_Attack_Weapon);
	// Since the client does not know who is a player and who is an enemy, just give some padding to the fireball spawn
	FVector deltaDistance = ProjectileTargetLocation - socketLocation;
	deltaDistance.Z = 0.0f;
	socketLocation += deltaDistance.GetSafeNormal() * this->SpawnPadding;
	socketLocation.Z -= this->SpawnDrop;
	
	FRotator rotation = (deltaDistance).Rotation();
	rotation.Pitch = 0.0; // Make projectile move flat

	FTransform spawnTransform;
	spawnTransform.SetLocation(socketLocation);
	spawnTransform.SetRotation(rotation.Quaternion());
	AAuraProjectile* projectile = this->GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		this->ProjectileClass,
		spawnTransform,
		this->GetOwningActorFromActorInfo(),
		Cast<APawn>(this->GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this->GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle effectContextHandle = sourceASC->MakeEffectContext();
	effectContextHandle.SetAbility(this);
	effectContextHandle.AddSourceObject(projectile);
	FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(this->DamageEffectClass, this->GetAbilityLevel(), effectContextHandle);

	// What is the point of doing it this way instead of just using the damage effect scalable float?
	// Sure this lets us use "Set by caller" but why??
	// Ooooh seems this will allow the ability itself to dictate the damage once we get it hooked up
	// We have added "Damage" to UAuraGameplayAbility which we can have the projectile use as the damage for the ability
	for (TPair<FGameplayTag, FScalableFloat>& pair : this->DamageTypes)
	{
		const float scaledDamage = pair.Value.GetValueAtLevel(this->GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, pair.Key, scaledDamage);
	}
		
	projectile->DamageEffectSpecHandle = specHandle;
	projectile->FinishSpawning(spawnTransform);
}
