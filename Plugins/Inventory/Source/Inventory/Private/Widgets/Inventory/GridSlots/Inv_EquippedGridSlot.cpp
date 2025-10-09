// Copyright - none


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_ItemTags.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"

void UInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!GetAvailable())
	{
		return;
	}

	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!IsValid(HoverItem))
	{
		return;
	}

	if (HoverItem->GetItemType().MatchesTag(FInventoryGameplayTags::Get().Equipment))
	{
		SetOccupiedTexture();
	}
}

void UInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!GetAvailable())
	{
		return;
	}

	UInv_HoverItem* HoverItem = UInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!IsValid(HoverItem))
	{
		return;
	}

	if (HoverItem->GetItemType().MatchesTag(FInventoryGameplayTags::Get().Equipment))
	{
		SetUnoccupiedTexture();
	}
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnFEquippedGridSlotClickedDelegate.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}
