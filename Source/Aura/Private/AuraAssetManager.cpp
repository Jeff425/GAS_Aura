// Copyright KhalorlStudios


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* assetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *assetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// Required for GAS
	UAbilitySystemGlobals::Get().InitGlobalData();
}
