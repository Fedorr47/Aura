// Copyright - none


#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Inv_InventoryItem.h"

void UInv_HoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	ImageIcon->SetBrush(Brush);
}

void UInv_HoverItem::UpdateStackCount(const int32 NewStackCount) const
{
	if (NewStackCount > 0)
	{
		TextStackCount->SetText(FText::AsNumber(NewStackCount));
		TextStackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		TextStackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag UInv_HoverItem::GetItemType() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem->GetItemManifest().GetItemType();
	}
	return FGameplayTag();
}

void UInv_HoverItem::SetIsStackable(bool bIsStacks)
{
	bIsStackable = bIsStacks;
	if (!bIsStacks)
	{
		TextStackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UInv_InventoryItem* UInv_HoverItem::GetInventoryItem() const
{
	return InventoryItem.Get();
}

void UInv_HoverItem::SetInventoryItem(UInv_InventoryItem* InInventoryItem)
{
	InventoryItem = InInventoryItem;
}
