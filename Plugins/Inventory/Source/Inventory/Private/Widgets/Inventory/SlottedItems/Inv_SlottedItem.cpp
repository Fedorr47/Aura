// Copyright - none


#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"

#include "Items/Inv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
