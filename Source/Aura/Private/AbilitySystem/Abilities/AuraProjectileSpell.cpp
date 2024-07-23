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

	ICombatInterface* combatInterface = Cast<ICombatInterface>(this->GetAvatarActorFromActorInfo());
	if (combatInterface)
	{
		const FVector socketLocation = combatInterface->GetCombatSocketLocation();
		FRotator rotation = (ProjectileTargetLocation - socketLocation).Rotation();
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
		FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(this->DamageEffectClass, this->GetAbilityLevel(), sourceASC->MakeEffectContext());

		// What is the point of doing it this way instead of just using the damage effect scalable float?
		// Sure this lets us use "Set by caller" but why??
		// Ooooh seems this will allow the ability itself to dictate the damage once we get it hooked up
		// We have added "Damage" to UAuraGameplayAbility which we can have the projectile use as the damage for the ability
		FAuraGameplayTags gameplayTags = FAuraGameplayTags::Get();
		//const float scaledDamage = this->Damage.GetValueAtLevel(this->GetAbilityLevel());
		const float scaledDamage = this->Damage.GetValueAtLevel(10.0);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(specHandle, gameplayTags.Damage, scaledDamage);

		projectile->DamageEffectSpecHandle = specHandle;
		projectile->FinishSpawning(spawnTransform);
	}
}
