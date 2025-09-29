#include "Items/Inv_ItemTags.h"

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

FInventoryGameplayTags FInventoryGameplayTags::Instance;

void FInventoryGameplayTags::InitializeNativeGameplayTags()
{
	// Secondary Attributes
	Instance.Equipment_Weapons_Axe = AddInputTag(
		"Axe",
		"Inventory", "GameItems", "Equipment", "Weapons", "Axe");

	Instance.Equipment_Weapons_Sword = AddInputTag(
		"Sword",
		"Inventory", "GameItems", "Equipment", "Weapons", "Sword");

	Instance.Equipment_Cloaks_RedCloak = AddInputTag(
		"Red cloak",
		"Inventory", "GameItems", "Equipment", "Cloaks", "RedCloak");

	Instance.Equipment_Masks_SteelMask = AddInputTag(
		"Steel mask",
		"Inventory", "GameItems", "Equipment", "Masks", "SteelMask");

	Instance.Consumables_Potions_Red_SmallPotion = AddInputTag(
		"Red small potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Red", "SmallPotion");
	Instance.Consumables_Potions_Red_LargePotion = AddInputTag(
		"Red large potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Red", "LargePotion");

	Instance.Consumables_Potions_Blue_SmallPotion = AddInputTag(
		"Blue small potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Blue", "SmallPotion");
	Instance.Consumables_Potions_Blue_LargePotion = AddInputTag(
		"Blue large potion",
		"Inventory", "GameItems", "Consumables", "Potions", "Blue", "LargePotion");

	Instance.Craftables_FireFernFruit = AddInputTag(
		"Fire fern fruit",
		"Inventory", "GameItems", "Craftables", "FireFernFruit");
	
	Instance.Craftables_LuminDaisy = AddInputTag(
		"Lumin daisy",
		"Inventory", "GameItems", "Craftables", "LuminDaisy");
	Instance.Craftables_ScorchPetalBlossom = AddInputTag(
		"Scorch petal blossom",
		"Inventory", "GameItems", "Craftables", "ScorchPetalBlossom");
}

