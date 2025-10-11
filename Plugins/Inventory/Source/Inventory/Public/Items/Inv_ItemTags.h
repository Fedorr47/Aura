#pragma once

#include "NativeGameplayTags.h"

/*
namespace GameItems
{
	namespace Equipment
	{
		namespace Weapon
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe);
		}
	}
}*/

/**
 * Inventory Tags
 *
 * Singleton containing native Inventory Tags
 */
struct INVENTORY_API FInventoryGameplayTags
{
public:
	static const FInventoryGameplayTags& Get();

	static void InitializeNativeGameplayTags();
	
	FGameplayTag Equipment_Weapons_Axe;
	FGameplayTag Equipment_Weapons_Sword;

	FGameplayTag Equipment_Cloaks_RedCloak;

	FGameplayTag Equipment_Masks_SteelMask;

	FGameplayTag Consumables_Potions_Red_SmallPotion;
	FGameplayTag Consumables_Potions_Red_LargePotion;

	FGameplayTag Consumables_Potions_Blue_SmallPotion;
	FGameplayTag Consumables_Potions_Blue_LargePotion;

	FGameplayTag Craftables_FireFernFruit;
	FGameplayTag Craftables_LuminDaisy;
	FGameplayTag Craftables_ScorchPetalBlossom;

private:
	static FInventoryGameplayTags ItemInstance;
};
