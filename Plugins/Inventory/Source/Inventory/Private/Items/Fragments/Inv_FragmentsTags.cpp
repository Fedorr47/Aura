#include "Items/Fragments/Inv_FragmentsTags.h"

#include "Utils/GeneralStatics.h"

FInventoryFragmentsTags FInventoryFragmentsTags::FragInstance;

const FInventoryFragmentsTags& FInventoryFragmentsTags::Get()
{
	return FragInstance;
}

void FInventoryFragmentsTags::InitializeNativeGameplayTags()
{
	FragInstance.GridFragment = FGeneralStatics::AddInputTag(
		"Grid fragment",
		"Inventory", "FragmentTags", "GridFragment");
	FragInstance.IconFragment = FGeneralStatics::AddInputTag(
		"Icon fragment",
		"Inventory", "FragmentTags", "IconFragment");
}