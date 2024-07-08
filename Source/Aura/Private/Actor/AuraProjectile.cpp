// Copyright KhalorlStudios


#include "Actor/AuraProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	this->bReplicates = true;
	this->Sphere = this->CreateDefaultSubobject<USphereComponent>("Sphere");
	this->SetRootComponent(this->Sphere);
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
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


