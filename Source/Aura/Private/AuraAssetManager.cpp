#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Items/Inv_ItemTags.h"

const UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine)
	const UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::Get().InitializeNativeGameplayTags();
	// TODO: Added from inventory plugin
	FInventoryGameplayTags::Get().InitializeNativeGameplayTags();
	//UAbilitySystemGlobals::Get().InitGlobalData();
}
