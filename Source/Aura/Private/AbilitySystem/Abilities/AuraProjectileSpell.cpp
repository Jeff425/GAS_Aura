// Copyright KhalorlStudios


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = this->HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* combatInterface = Cast<ICombatInterface>(this->GetAvatarActorFromActorInfo());
	if (combatInterface)
	{
		const FVector socketLocation = combatInterface->GetCombatSocketLocation();
		FTransform spawnTransform;
		spawnTransform.SetLocation(socketLocation);
		// TODO: Set Rotation
		AAuraProjectile* projectile = this->GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			this->ProjectileClass,
			spawnTransform,
			this->GetOwningActorFromActorInfo(),
			Cast<APawn>(this->GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		// TODO: Give projectile a Gameplay Effect Spec for Damage
		projectile->FinishSpawning(spawnTransform);
	}
}
