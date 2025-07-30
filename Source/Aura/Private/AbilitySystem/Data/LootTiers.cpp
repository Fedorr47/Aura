// Copyright - none


#include "AbilitySystem/Data/LootTiers.h"

TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;

	for (FLootItem& Item : LootItems)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			if (FMath::FRandRange(1.0f, 100.0f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLoottLevelOverride = Item.bLoottLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}

	return ReturnItems;
}
