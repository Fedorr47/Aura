#include "Items/Fragments/Inv_ItemFragment.h"

#include "InventoryManager/Components/Inv_InventoryComponent.h"

void FInv_ConsumableFragment::OnConsume(const UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnHealthEffectDelegate.Broadcast(0);
}

void FInv_HealthPotionFragment::OnConsume(
	const UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnHealthEffectDelegate.Broadcast(HealAmount);
}
