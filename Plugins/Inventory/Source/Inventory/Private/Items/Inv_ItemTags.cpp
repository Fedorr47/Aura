#include "Items/Inv_ItemTags.h"
#include "Utils/GeneralStatics.h"

/*
namespace GameItems
{
	namespace Equipment
	{
		namespace Weapon
		{
			UE_DEFINE_GAMEPLAY_TAG(Axe, "GameItems.Equipment.Weapon.Axe", "Axe");
		}
	}
}*/

FInventoryGameplayTags FInventoryGameplayTags::ItemInstance;

const FInventoryGameplayTags& FInventoryGameplayTags::Get()
{
	return ItemInstance;
}

void FInventoryGameplayTags::InitializeNativeGameplayTags()
{
	ItemInstance.Equipment_Weapons_Axe = FGeneralStatics::AddInputTag(
		"Axe",
		"Inventory", "GameItems", "Equipment", "Weapons", "Axe");

	ItemInstance.Equipment_Weapons_Sword = FGeneralStatics::AddInputTag(
		"Sword",
		"Inventory", "GameItems", "Equipment", "Weapons", "Sword");

	ItemInstance.Equipment_Cloaks_RedCloak = FGeneralStatics::AddInputTag(
		"Red cloak",
		"Inventory", "GameItems", "Equipment", "Cloaks", "RedCloak");
	ItemInstance.Equipment_Cloaks_BlueCloak = FGeneralStatics::AddInputTag(
		"Blue cloak",
		"Inventory", "GameItems", "Equipment", "Cloaks", "BlueCloak");

	ItemInstance.Equipment_Masks_SteelMask = FGeneralStatics::AddInputTag(
		"Steel mask",
		"Inventory", "GameItems", "Equipment", "Masks", "SteelMask");

	ItemInstance.Consumables_Potions_Red_SmallPotion = FGeneralStatics::AddInputTag(
		"Red small potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Red", "SmallPotion");
	ItemInstance.Consumables_Potions_Red_LargePotion = FGeneralStatics::AddInputTag(
		"Red large potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Red", "LargePotion");

	ItemInstance.Consumables_Potions_Blue_SmallPotion = FGeneralStatics::AddInputTag(
		"Blue small potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Blue", "SmallPotion");
	ItemInstance.Consumables_Potions_Blue_LargePotion = FGeneralStatics::AddInputTag(
		"Blue large potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Blue", "LargePotion");

	ItemInstance.Craftables_FireFernFruit = FGeneralStatics::AddInputTag(
		"Fire fern fruit",
		"Inventory", "GameItems", "Craftables", "FireFernFruit");
	
	ItemInstance.Craftables_LuminDaisy = FGeneralStatics::AddInputTag(
		"Lumin daisy",
		"Inventory", "GameItems", "Craftables", "LuminDaisy");
	ItemInstance.Craftables_ScorchPetalBlossom = FGeneralStatics::AddInputTag(
		"Scorch petal blossom",
		"Inventory", "GameItems", "Craftables", "ScorchPetalBlossom");
}

