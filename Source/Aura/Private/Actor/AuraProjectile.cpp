// Copyright KhalorlStudios


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Aura/Aura.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	this->bReplicates = true;
	this->Sphere = this->CreateDefaultSubobject<USphereComponent>("Sphere");
	this->SetRootComponent(this->Sphere);
	this->Sphere->SetCollisionObjectType(ECC_Projectile);
	this->Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	this->Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	this->Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	this->ProjectileMovement = this->CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	this->ProjectileMovement->InitialSpeed = 550.0;
	this->ProjectileMovement->MaxSpeed = 550.0;
	this->ProjectileMovement->ProjectileGravityScale = 0.0;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
	if (this->LoopingSound)
	{
		this->LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(this->LoopingSound, this->GetRootComponent());
	}
	this->SetLifeSpan(this->Lifespan);
}

void AAuraProjectile::Destroyed()
{
	// If the client has not overlapped but is sent the destroy event, then play the destroy effects
	if (!this->bHit && !this->HasAuthority())
	{
		this->PlayImpactSoundAndEffect();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->PlayImpactSoundAndEffect();
	if (this->HasAuthority())
	{
		if (UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// The target will hit itself instead of being hit by caster?? hmm
			targetASC->ApplyGameplayEffectSpecToSelf(*this->DamageEffectSpecHandle.Data.Get());
		}
		this->Destroy();
	}
	else
	{
		this->bHit = true;
	}
}

void AAuraProjectile::PlayImpactSoundAndEffect()
{
	if (this->ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, this->ImpactSound, this->GetActorLocation(), FRotator::ZeroRotator);
	}
	if (this->ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, this->ImpactEffect, this->GetActorLocation());
	}
	if (this->LoopingSoundComponent)
	{
		this->LoopingSoundComponent->Stop();
	}
}


