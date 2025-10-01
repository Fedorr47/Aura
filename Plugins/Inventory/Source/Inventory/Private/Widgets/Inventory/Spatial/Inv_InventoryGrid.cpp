// Copyright - none

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Inventory/Inv_GridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentsTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &ThisClass::AddItem);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

inline FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const FInv_ItemManifest& Manifest)
{
	FInv_SlotAvailabilityResult AvailabilityResult;

	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	AvailabilityResult.bStackable = StackableFragment != nullptr;

	const int32 MaxStackCount = AvailabilityResult.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = AvailabilityResult.bStackable ? StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;
	for (const TObjectPtr<UInv_GridSlot>& GridSlot : GridSlots)
	{
		if (AmountToFill == 0)
		{
			break;
		}
		if (IsIndexClaimed(CheckedIndices, GridSlot->GetTileIndex()))
		{
			continue;
		}

		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(
			GridSlot,
			GetItemDimensions(Manifest),
			CheckedIndices,
			TentativelyClaimed,
			Manifest.GetItemType(),
			MaxStackCount))
		{
			continue;
		}

		CheckedIndices.Append(TentativelyClaimed);
	}
	
	return AvailabilityResult;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
}

bool UInv_InventoryGrid::HasRoomAtIndex(
	const UInv_GridSlot* GridSlot,
	const FIntPoint& Dimensions,
	TSet<int32>& CheckedIndecies,
	TSet<int32>& OutTentativelyClaimed,
	const FGameplayTag& ItemType,
	const int32 MaxStackSize)
{
	bool bHasRoomAtIndex = true;

	UInv_InventoryStatics::ForEach2D(GridSlots,GridSlot->GetTileIndex(),Dimensions,Columns,
		[&](const UInv_GridSlot* SubGridSlot)
		{
			if (CheckSlotConstraints(
				GridSlot,
				SubGridSlot,
				CheckedIndecies,
				OutTentativelyClaimed,
				ItemType,
				MaxStackSize))
			{
				OutTentativelyClaimed.Add(SubGridSlot->GetTileIndex());
			}
			else
			{
				bHasRoomAtIndex = false;
			}
		});
	
	return bHasRoomAtIndex;
}

bool UInv_InventoryGrid::CheckSlotConstraints(
	const UInv_GridSlot* GridSlot,
	const UInv_GridSlot* SubGridSlot,
	const TSet<int32>& CheckeIndecies,
	TSet<int32>& OutTentativelyClaimed,
	const FGameplayTag& ItemType,
	const int32 MaxStackSize) const
{
	if (IsIndexClaimed(CheckeIndecies, SubGridSlot->GetTileIndex()))
	{
		return false;
	}
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetTileIndex());
		return true;
	}
	if (!IsUpperLeftSlot(GridSlot, SubGridSlot))
	{
		return false;
	}
	const UInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable())
	{
		return false;
	}
	if (!DoesItemMatch(SubItem, ItemType))
	{
		return false;
	}
	if (GridSlot->GetStackCount() >= MaxStackSize)
	{
		return false;
	}
	return true;
}

bool UInv_InventoryGrid::IsUpperLeftSlot(const UInv_GridSlot* GridSlot, const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftIndex() == GridSlot->GetTileIndex();
}

bool UInv_InventoryGrid::DoesItemMatch(const UInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool UInv_InventoryGrid::HasValidItem(const UInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetInventoryItem().IsValid();
}

bool UInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* InItem)
{
	if (!MatchCategory(InItem))
	{
		return;
	}

	FInv_SlotAvailabilityResult AvailabilityResult = HasRoomForItem(InItem);
	AddItemToIndecies(AvailabilityResult, InItem);
}

void UInv_InventoryGrid::AddItemToIndecies(
	const FInv_SlotAvailabilityResult& AvailabilityResult,
	UInv_InventoryItem* InItem)
{
	for (const auto& SlotAvailability : AvailabilityResult.SlotAvailabilities)
	{
		AddItemAtIndex(InItem, SlotAvailability.Index, AvailabilityResult.bStackable, SlotAvailability.AmountToFill);
		UpdateGridSlots(InItem, SlotAvailability.Index, AvailabilityResult.bStackable, SlotAvailability.AmountToFill);
	}
}

void UInv_InventoryGrid::AddItemAtIndex(
	UInv_InventoryItem* InItem,
	const int32 Index,
	const bool bStackable,
	const int32 StackAmount)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(
		InItem, FInventoryFragmentsTags::Get().GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(
		InItem, FInventoryFragmentsTags::Get().IconFragment);

	if (!GridFragment || !ImageFragment)
	{
		return;
	}

	UInv_SlottedItem* SlottedItem = CreateSlottedItem(
		InItem,
		bStackable,
		StackAmount,
		GridFragment,
		ImageFragment,
		Index);
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);
	SlottedItems.Add(Index, SlottedItem);
}

UInv_SlottedItem* UInv_InventoryGrid::CreateSlottedItem(
	UInv_InventoryItem* InItem,
	const bool bStackable,
	const int32 StackAmount,
	const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment,
	const int32 Index) const
{
	UInv_SlottedItem* SlottedItem = CreateWidget<UInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(InItem);
	SetSlottedItemImage(SlottedItem, GridFragment, ImageFragment);
	SlottedItem->SetGridIndex(Index);
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);

	return SlottedItem;
}

void UInv_InventoryGrid::AddSlottedItemToCanvas(const int32 Index, const FInv_GridFragment* GridFragment,
	UInv_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));
	const FVector2D DrawPos = UInv_WidgetUtils::GetPositionFromIndex(Index, Columns) * TileSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void UInv_InventoryGrid::UpdateGridSlots(
	UInv_InventoryItem* InItem,
	const int32 Index,
	bool bStackable,
	const int32 StackAmount)
{
	check(GridSlots.IsValidIndex(Index));

	if (bStackable)
	{
		GridSlots[Index]->SetStackCount(StackAmount);
	}

	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(
		InItem, FInventoryFragmentsTags::Get().GridFragment);
	if (!GridFragment)
	{
		return;
	}

	const FIntPoint Dimension = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);

	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimension, Columns,
		[&](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(InItem);
			GridSlot->SetUpperLeftIndex(Index);
			GridSlot->SetOccupiedTexture();
			GridSlot->SetAvailable(false);
		});
}

FVector2D UInv_InventoryGrid::GetDrawSize(const FInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = TileSize * GridFragment->GetGridPadding();
	return GridFragment->GetGridSize() * IconTileWidth;
}

void UInv_InventoryGrid::SetSlottedItemImage(
	const UInv_SlottedItem* SlottedItem,
	const FInv_GridFragment* GridFragment,
	const FInv_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

void UInv_InventoryGrid::ConstructGrid()
{
	GridSlots.Reserve(Rows * Columns);

	for (int i = 0; i < Rows; i++)
	{
		for (int j = 0; j < Columns; j++)
		{
			UInv_GridSlot* GridSlot = CreateWidget<UInv_GridSlot>(this, GridSlotClass);
			CanvasPanel->AddChild(GridSlot);

			const FIntPoint TilePosition = FIntPoint(j, i);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCanvasPanelSlot->SetSize(FVector2d(TileSize));
			GridCanvasPanelSlot->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
		}
	}
}

bool UInv_InventoryGrid::MatchCategory(const UInv_InventoryItem* InItem) const
{
	return InItem->GetItemManifest().GetItemCategory() == ItemCategory;
}
