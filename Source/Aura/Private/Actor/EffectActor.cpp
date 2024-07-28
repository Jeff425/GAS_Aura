// Copyright KhalorlStudios


#include "Actor/EffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include <AbilitySystemBlueprintLibrary.h>


AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	this->SetRootComponent(this->CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// Check if the target is an enemy AND if the effect should be applied to enemies
	if (TargetActor->ActorHasTag(FName("Enemy")) && !this->bApplyEffectsToEnemies) return;
	check(GameplayEffectClass);
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (IsValid(targetASC))
	{
		FGameplayEffectContextHandle contextHandle = targetASC->MakeEffectContext();
		contextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle specHandle = targetASC->MakeOutgoingSpec(GameplayEffectClass, this->ActorLevel, contextHandle);
		FActiveGameplayEffectHandle activeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
		switch (specHandle.Data.Get()->Def.Get()->DurationPolicy)
		{
			case EGameplayEffectDurationType::HasDuration:
				if (this->DurationEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
				{
					// Note: This is probably NOT the correct way to associate with the object. These are reused!
					this->DurationEffectHandles.Add(targetASC->GetUniqueID(), activeHandle);
				}
				
				break;
			case EGameplayEffectDurationType::Infinite:
				if (this->InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
				{
					this->InfiniteEffectHandles.Add(targetASC->GetUniqueID(), activeHandle);
				}
				break;
		}
		// If it is an instant effect AND this is suppose to destroy itself, do so
		if (this->bDestroyOnEffectApplication && specHandle.Data.Get()->Def.Get()->DurationPolicy != EGameplayEffectDurationType::Infinite)
		{
			this->Destroy();
		}
	}
}

void AEffectActor::OnOverlap(AActor* TargetActor)
{
	// Check if the target is an enemy AND if the effect should be applied to enemies
	if (TargetActor->ActorHasTag(FName("Enemy")) && !this->bApplyEffectsToEnemies) return;
	if (this->InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->InstantGameplayEffectClass);
	}

	if (this->DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->DurationGameplayEffectClass);
	}

	if (this->InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->InfiniteGameplayEffectClass);
	}
}

void AEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// Check if the target is an enemy AND if the effect should be applied to enemies
	if (TargetActor->ActorHasTag(FName("Enemy")) && !this->bApplyEffectsToEnemies) return;
	if (this->InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->InstantGameplayEffectClass);
	}

	if (this->DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->DurationGameplayEffectClass);
	}
	if (this->DurationEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(targetASC))
		{
			FActiveGameplayEffectHandle activeHandle;
			if (this->DurationEffectHandles.RemoveAndCopyValue(targetASC->GetUniqueID(), activeHandle))
			{
				targetASC->RemoveActiveGameplayEffect(activeHandle);
			}
		}
	}

	if (this->InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		this->ApplyEffectToTarget(TargetActor, this->InfiniteGameplayEffectClass);
	}
	if (this->InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (IsValid(targetASC))
		{
			FActiveGameplayEffectHandle activeHandle;
			if (this->InfiniteEffectHandles.RemoveAndCopyValue(targetASC->GetUniqueID(), activeHandle))
			{
				targetASC->RemoveActiveGameplayEffect(activeHandle);
			}
		}
	}
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();

}


