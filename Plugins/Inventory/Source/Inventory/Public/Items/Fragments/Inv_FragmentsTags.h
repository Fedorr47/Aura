#pragma once

#include "NativeGameplayTags.h"

struct INVENTORY_API FInventoryFragmentsTags
{
public:
	static const FInventoryFragmentsTags& Get();

	static void InitializeNativeGameplayTags();

	FGameplayTag GridFragment;
	FGameplayTag IconFragment;
	FGameplayTag StackableFragment;
	FGameplayTag ConsumableFragment;

	FGameplayTag ItemNameFragment;
	FGameplayTag PrimaryStatFragment;

	FGameplayTag ItemTypeFragment;
	FGameplayTag FlavorTextFragment;
	FGameplayTag SellValueFragment;
	FGameplayTag RequiredLevelFragment;

	FGameplayTag StatModifier_1;
	FGameplayTag StatModifier_2;
	FGameplayTag StatModifier_3;

private:
	static FInventoryFragmentsTags FragInstance;
};
