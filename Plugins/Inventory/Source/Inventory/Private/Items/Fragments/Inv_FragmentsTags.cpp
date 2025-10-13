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
	FragInstance.StackableFragment = FGeneralStatics::AddInputTag(
		"Stackable fragment",
		"Inventory", "FragmentTags", "StackableFragment");
	FragInstance.ConsumableFragment = FGeneralStatics::AddInputTag(
		"Consumable fragment",
		"Inventory", "FragmentTags", "ConsumableFragment");
	FragInstance.EquipmentFragment = FGeneralStatics::AddInputTag(
		"Equipment fragment",
		"Inventory", "FragmentTags", "EquipmentFragment");

	FragInstance.ItemNameFragment = FGeneralStatics::AddInputTag(
		"Item name Fragment",
		"Inventory", "FragmentTags", "ItemNameFragment"
		);
	FragInstance.PrimaryStatFragment = FGeneralStatics::AddInputTag(
		"Primary stat fragment",
		"Inventory", "FragmentTags", "PrimaryStatFragment");

	FragInstance.StatModifier_1 = FGeneralStatics::AddInputTag(
		"Stat modifier 1",
		"Inventory", "FragmentTags", "StatModifier" , "1");
	FragInstance.StatModifier_2 = FGeneralStatics::AddInputTag(
		"Stat modifier 2",
		"Inventory", "FragmentTags", "StatModifier", "2");
	FragInstance.StatModifier_3 = FGeneralStatics::AddInputTag(
		"Stat modifier 3",
		"Inventory", "FragmentTags", "StatModifier", "3");

	FragInstance.ItemTypeFragment = FGeneralStatics::AddInputTag(
		"Item type fragment",
		"Inventory", "FragmentTags", "ItemTypeFragment");
	FragInstance.FlavorTextFragment = FGeneralStatics::AddInputTag(
		"Flavor text fragment",
		"Inventory", "FragmentTags", "FlavorTextFragment");
	FragInstance.SellValueFragment= FGeneralStatics::AddInputTag(
		"Sell value fragment",
		"Inventory", "FragmentTags", "SellValueFragment");
	FragInstance.RequiredLevelFragment = FGeneralStatics::AddInputTag(
		"Required level fragment",
		"Inventory", "FragmentTags", "RequiredLevelFragment");
}