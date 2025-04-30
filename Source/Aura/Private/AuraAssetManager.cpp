// Copyright - none


#include "AuraAssetManager.h"

#include "AuraGameplayTags.h"

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
}
