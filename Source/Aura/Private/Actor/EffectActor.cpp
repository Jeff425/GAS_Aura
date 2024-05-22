// Copyright KhalorlStudios


#include "Actor/EffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	this->Sphere = this->CreateDefaultSubobject<USphereComponent>("Sphere");
	this->Mesh = this->CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	this->SetRootComponent(this->Mesh);
	this->Sphere->SetupAttachment(this->GetRootComponent());

}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	this->Sphere->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnOverlap);
	this->Sphere->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::EndOverlap);
}

void AEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change this to apply a Gameplay Effect. For now casting away const
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet* auraAttributeSet = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet* mutSet = const_cast<UAuraAttributeSet*>(auraAttributeSet);
		mutSet->SetHealth(auraAttributeSet->GetHealth() + 25.0);
		this->Destroy();
	}
}

void AEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


