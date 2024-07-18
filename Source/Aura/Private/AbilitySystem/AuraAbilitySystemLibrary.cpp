// Copyright KhalorlStudios


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include <Kismet/GameplayStatics.h>
#include <UI/HUD/AuraHUD.h>
#include <Player/AuraPlayerState.h>
#include "UI/Controller/BaseWidgetController.h"
#include <Game/AuraGMBase.h>

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// Calling from a widget, which means called from a local player
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* auraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->AttributeSet;
			const FWidgetControllerParams params(PC, PS, ASC, AS);
			return auraHUD->GetOverlayWidgetController(params);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	// Calling from a widget, which means called from a local player
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* auraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->AttributeSet;
			const FWidgetControllerParams params(PC, PS, ASC, AS);
			return auraHUD->GetAttributeMenuWidgetController(params);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass ChartacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGMBase* auraGM = Cast<AAuraGMBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!auraGM) return;

	AActor* avatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* classInfo = auraGM->CharacterClassInfo;
	FCharacterClassDefaultInfo classDefaultInfo = classInfo->GetClassDefaultInfo(ChartacterClass);
	
	FGameplayEffectContextHandle contextHandle = ASC->MakeEffectContext();
	contextHandle.AddSourceObject(avatarActor);
	// Instant Effect
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(classDefaultInfo.PrimaryAttributes, Level, contextHandle).Data.Get());

	// These are all infinite effects
	// Defines the relationship between primary to secondary/vital
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(classInfo->SecondaryAttributes, Level, contextHandle).Data.Get());
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(classInfo->VitalAttributes, Level, contextHandle).Data.Get());
}
