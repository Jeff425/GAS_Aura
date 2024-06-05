// Copyright KhalorlStudios


#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include <AbilitySystemBlueprintLibrary.h>

UAuraAttributeSet::UAuraAttributeSet()
{

}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Health will be replicated to
	//COND_None: All clients
	//REPNOTIFY_Always: Triggers replication even if the value has not changed. Useful if we want to trigger the callback
	// Vitials
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Primary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Crit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CritResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
}

// Incorrect time to clamp, clamps querying modifiers or something?
// Unsure why I would clamp here at all
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == this->GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0, this->GetMaxHealth());
	}

	if (Attribute == this->GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0, this->GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Source = Causer of effect, Target = owner of THIS attribute set (target of effect)

	// From source ASC, can fetch AbilityActorInfo to get Avatar, PlayerController Etc.
	// Look at BuildEffectProperties for an example of different information to fetch

	// Correct time to clamp
	if (Data.EvaluatedData.Attribute == this->GetHealthAttribute())
	{
		this->SetHealth(FMath::Clamp(this->GetHealth(), 0.0, this->GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == this->GetManaAttribute())
	{
		this->SetMana(FMath::Clamp(this->GetMana(), 0.0, this->GetMaxMana()));
	}
}

// Fetches various information from Effect Callback Data
FEffectProperties UAuraAttributeSet::BuildEffectProperties(const FGameplayEffectModCallbackData& Data) const
{
	FEffectProperties Props;
	Props.ContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.ContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid())
	{
		if (Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
		{
			Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		}
		if (Props.SourceASC->AbilityActorInfo->PlayerController.IsValid())
		{
			Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		}
		else
		{
			if (const APawn* pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
	return Props;
}


void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldVal);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldVal);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldVal);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldVal);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldVal);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldVal);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldVal);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVal);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldVal);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldVal);
}

void UAuraAttributeSet::OnRep_Crit(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Crit, OldVal);
}

void UAuraAttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritDamage, OldVal);
}

void UAuraAttributeSet::OnRep_CritResistance(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CritResistance, OldVal);
}

void UAuraAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegen, OldVal);
}

void UAuraAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldVal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegen, OldVal);
}
