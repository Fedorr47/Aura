// Copyright - none


#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"

#include "Items/Inv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"

FReply UInv_SlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClickedDelegate.Broadcast(GridIndex, InMouseEvent);
	return FReply::Handled();
}

void UInv_SlottedItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInv_InventoryStatics::ItemHovered(GetOwningPlayer(), InventoryItem.Get());
}

void UInv_SlottedItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());
}

void UInv_SlottedItem::SetInventoryItem(UInv_InventoryItem* Item)
{
	InventoryItem = Item;
}
void UInv_SlottedItem::SetImageBrush(const FSlateBrush& Brush) const
{
	ImageIcon->SetBrush(Brush);
}

void UInv_SlottedItem::UpdateStackCount(int32 InStackCount)
{
	if (InStackCount > 0)
	{
		TextStackCount->SetVisibility(ESlateVisibility::Visible);
		TextStackCount->SetText(FText::AsNumber(InStackCount));
	}
	else
	{
		TextStackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
