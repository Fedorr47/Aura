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

private:
	static FInventoryFragmentsTags FragInstance;
};
