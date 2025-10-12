// Copyright - none


#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Inv_ItemTags.h"
#include "Items/Fragments/Inv_FragmentsTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"

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

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetOccupiedTexture();
		ImageGrayOutIcon->SetVisibility(ESlateVisibility::Collapsed);
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

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag))
	{
		SetUnoccupiedTexture();
		ImageGrayOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply UInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnEquippedGridSlotClickedDelegate.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}

UInv_EquippedSlottedItem* UInv_EquippedGridSlot::OnItemEquipped(
	UInv_InventoryItem* InInventoryItem,
	const FGameplayTag& InEquippedTypeTag,
	float TileSize)
{
	if (!EquipmentTypeTag.MatchesTagExact(InEquippedTypeTag))
	{
		return nullptr;
	}
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(InInventoryItem, FInventoryFragmentsTags::Get().GridFragment);
	if (!GridFragment)
	{
		return nullptr;
	}
	const FIntPoint GridDimension = GridFragment->GetGridSize();
	
	const float IconTileWidth  = TileSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimension * IconTileWidth ;

	EquippedSlottedItemWidget = CreateWidget<UInv_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);
	EquippedSlottedItemWidget->SetInventoryItem(InInventoryItem);
	EquippedSlottedItemWidget->SetEquippedSlottedItemType(InEquippedTypeTag);
	EquippedSlottedItemWidget->UpdateStackCount(0);
	SetInventoryItem(InInventoryItem);

	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(InInventoryItem, FInventoryFragmentsTags::Get().IconFragment);
	if (!ImageFragment)
	{
		return nullptr;
	}

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;

	EquippedSlottedItemWidget->SetImageBrush(Brush);

	OverlayRoot->AddChildToOverlay(EquippedSlottedItemWidget);
	FGeometry OverlayGeometry = OverlayRoot->GetCachedGeometry();
	//auto OverlayPosition = OverlayGeometry.Position;
	FVector2D OverlaySize = OverlayGeometry.Size;

	const float LeftPadding = OverlaySize.X / 2.0f - DrawSize.X / 2.0f;
	const float TopPadding = OverlaySize.Y / 2.0f - DrawSize.Y / 2.0f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItemWidget);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));
	
	return EquippedSlottedItemWidget;
}
