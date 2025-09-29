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
	static const FInventoryGameplayTags& Get()
	{
		return Instance;
	}

	static void InitializeNativeGameplayTags();

	template<typename... Parts>
	static FName MakeTagName(Parts&&... parts)
	{
		FString TagString;
		((TagString += FString(parts) + TEXT(".")), ...);
		TagString.RemoveFromEnd(TEXT("."));
		return FName(TagString);
	}

	template<typename... Parts>
	static FGameplayTag AddInputTag(const FString& TagComment, Parts&&... parts)
	{
		return UGameplayTagsManager::Get().AddNativeGameplayTag(
			MakeTagName(Forward<Parts>(parts)...),
			TagComment
		);
	}

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
	static FInventoryGameplayTags Instance;
	
	FInventoryGameplayTags() = default;
	~FInventoryGameplayTags() = default;
};
