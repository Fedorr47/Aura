// Copyright - none

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"

#include "Inventory.h"
#include "Widgets/Inventory/GridSlots/Inv_GridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_FragmentsTags.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPosition = UInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D CanvasSize = UInv_WidgetUtils::GetWidgetSize(CanvasPanel);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPosition, CanvasSize, MousePosition))
	{
		return;
	}
	
	UpdateTileParameters(CanvasPosition, MousePosition);
}

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &ThisClass::AddItem);
	InventoryComponent->OnStackChangedDelegate.AddDynamic(this, &ThisClass::AddStacks);
	InventoryComponent->OnInventoryMenuToggledDelegate.AddDynamic(this, &ThisClass::OnInventoryMenuToggled);
}

void UInv_InventoryGrid::SetOwningCanvasPanel(UCanvasPanel* InOwningCanvasPanel)
{
	OwningCanvasPanel = InOwningCanvasPanel;
}

bool UInv_InventoryGrid::CursorExitedCanvas(
	const FVector2D& BoundaryPosition,
	const FVector2D& BoundarySize,
	const FVector2D& Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = UInv_WidgetUtils::IsWithinBounds(BoundaryPosition, BoundarySize, Location);
	if (!bMouseWithinCanvas && bLastMouseWithinCanvas)
	{
		UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
		return true;
	}
	return false;
}

void UInv_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	if (!bMouseWithinCanvas)
	{
		return;
	}

	UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns,
		[&](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetOccupiedTexture();
		});
	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

void UInv_InventoryGrid::UnhighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns,
		[&](UInv_GridSlot* GridSlot)
		{
			if (GridSlot->GetAvailable())
			{
				GridSlot->SetUnoccupiedTexture();
			}
			else
			{
				GridSlot->SetOccupiedTexture();
			}
		});
}

void UInv_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition)
{
	if (!bMouseWithinCanvas)
	{
		return;
	}
	
	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPosition, MousePosition);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = UInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPosition, MousePosition);

	OnTileParametersUpdated(TileParameters);
}

void UInv_InventoryGrid::OnTileParametersUpdated(const FInv_TileParameters& Parameters)
{
	if (!IsValid(HoverItem))
	{
		return;
	}

	const FIntPoint Dimensions = HoverItem->GetGridDimension();
	const FIntPoint StartingCoordinates = CalculateStartingCoordinates(
		Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = UInv_WidgetUtils::GetIndexFromPosition(StartingCoordinates, Columns);

	CurrentQueryResult = CheckHoverPosition(StartingCoordinates, Dimensions);
	if (CurrentQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		return;
	}
	UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(
			CurrentQueryResult.ValidItem.Get(), FInventoryFragmentsTags::Get().GridFragment);
		if (!GridFragment)
		{
			return;
		}

		ChangeHoverType(
			CurrentQueryResult.UpperLeftIndex,
			GridFragment->GetGridSize(),
			EInv_GridSlotState::GrayedOut);
	}
}

void UInv_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions,
	EInv_GridSlotState GridSlotState)
{
	UnhighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	UInv_InventoryStatics::ForEach2D(GridSlots, Index, Dimensions, Columns,
		[GridSlotState](UInv_GridSlot* GridSlot)
		{
			switch (GridSlotState)
			{
			case EInv_GridSlotState::Occupied:
				GridSlot->SetOccupiedTexture();
				break;
			case EInv_GridSlotState::Unoccupied:
				GridSlot->SetUnoccupiedTexture();
				break;
			case EInv_GridSlotState::Selected:
				GridSlot->SetSelectedTexture();
				break;
			case EInv_GridSlotState::GrayedOut:
				GridSlot->SetGrayedOutTexture();
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Invalid GridSlotState"));
			}
		});

	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

FInv_SpaceQueryResult UInv_InventoryGrid::CheckHoverPosition(const FIntPoint& Position,
                                                             const FIntPoint& Dimensions) 
{
	FInv_SpaceQueryResult QueryResult;

	int32 HoveredTilePosition = UInv_WidgetUtils::GetIndexFromPosition(Position, Columns);
	if (!IsInGridBounds(HoveredTilePosition, Dimensions))
	{
		return QueryResult;
	}

	QueryResult.bHasSpace = true;

	TSet<int32> OccupiedUpperLeftIndexes;
	UInv_InventoryStatics::ForEach2D(GridSlots, HoveredTilePosition, Dimensions, Columns,
		[&](const UInv_GridSlot* GridSlot)
		{
			if (GridSlot->GetInventoryItem().IsValid())
			{
				OccupiedUpperLeftIndexes.Add(GridSlot->GetUpperLeftIndex());
				QueryResult.bHasSpace = false;
			}
		});

	if (OccupiedUpperLeftIndexes.Num() == 1)
	{
		const int32 Index = *OccupiedUpperLeftIndexes.CreateConstIterator();
		QueryResult.ValidItem = GridSlots[Index]->GetInventoryItem();
		QueryResult.UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	}
	
	return QueryResult;
}

FIntPoint UInv_InventoryGrid::CalculateStartingCoordinates(
	const FIntPoint& Coordinates,
	const FIntPoint& Dimension,
	EInv_TileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimension.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight = Dimension.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoordinates{};
	switch (Quadrant)
	{
		case EInv_TileQuadrant::TopLeft:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimension.X);
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimension.Y);
			break;
		case EInv_TileQuadrant::TopRight:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimension.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimension.Y);
			break;
		case EInv_TileQuadrant::BottomLeft:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimension.X);
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimension.Y) + HasEvenHeight;
			break;
		case EInv_TileQuadrant::BottomRight:
			StartingCoordinates.X = Coordinates.X - FMath::FloorToInt(0.5f * Dimension.X) + HasEvenWidth;
			StartingCoordinates.Y = Coordinates.Y - FMath::FloorToInt(0.5f * Dimension.Y) + HasEvenHeight;
			break;
		default:
			UE_LOG(LogInventory, Warning, TEXT("Invalid Quadrant"));
			StartingCoordinates = FIntPoint{-1, -1};
	}
	
	return StartingCoordinates;
}

FIntPoint UInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPosition,
                                                          const FVector2D& MousePosition) const
{
	return FIntPoint {
		static_cast<int32>(FMath::FloorToInt(MousePosition.X - CanvasPosition.X) / TileSize),
		static_cast<int32>(FMath::FloorToInt(MousePosition.Y - CanvasPosition.Y) / TileSize)
	};
}

EInv_TileQuadrant UInv_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPosition,
	const FVector2D& MousePosition) const
{
	const float TileLocalX = FMath::Fmod((MousePosition.X - CanvasPosition.X), TileSize);
	const float TileLocalY = FMath::Fmod((MousePosition.Y - CanvasPosition.Y), TileSize);

	const bool bIsTop = TileLocalY < TileSize / 2.0f;
	const bool bIsLeft = TileLocalX < TileSize / 2.0f;

	EInv_TileQuadrant HoveredTileQuadrant = EInv_TileQuadrant::None;
	if (bIsTop && bIsLeft)
	{
		HoveredTileQuadrant = EInv_TileQuadrant::TopLeft;
	}
	else if (bIsTop && !bIsLeft)
	{
		HoveredTileQuadrant = EInv_TileQuadrant::TopRight;
	}
	else if (bIsLeft && !bIsTop)
	{
		HoveredTileQuadrant = EInv_TileQuadrant::BottomLeft;
	}
	else if (!bIsTop && !bIsLeft)
	{
		HoveredTileQuadrant = EInv_TileQuadrant::BottomRight;
	}

	return HoveredTileQuadrant;
}


FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(const UInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

inline FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(
	const UInv_InventoryItem* Item,
	const int32 StackAmountOverride /*= -1*/)
{
	return HasRoomForItem(Item->GetItemManifest(), StackAmountOverride);
}

FInv_SlotAvailabilityResult UInv_InventoryGrid::HasRoomForItem(
	const FInv_ItemManifest& Manifest,
	const int32 StackAmountOverride /*= -1*/)
{
	FInv_SlotAvailabilityResult AvailabilityResult;

	const FInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FInv_StackableFragment>();
	AvailabilityResult.bStackable = StackableFragment != nullptr;

	const int32 MaxStackSize = AvailabilityResult.bStackable ? StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = AvailabilityResult.bStackable ? StackableFragment->GetStackCount() : 1;
	if (StackAmountOverride != -1 && AvailabilityResult.bStackable)
	{
		AmountToFill = StackAmountOverride;
	}

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

		if (!IsInGridBounds(GridSlot->GetTileIndex(), GetItemDimensions(Manifest)))
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
			MaxStackSize))
		{
			continue;
		}

		const int32 AmountToFillSlot = DetermineFillAmountForSlot(
			AvailabilityResult.bStackable, MaxStackSize, AmountToFill, GridSlot);
		if (AmountToFillSlot == 0)
		{
			continue;
		}

		CheckedIndices.Append(TentativelyClaimed);

		AvailabilityResult.TotalRoomToFill += AmountToFillSlot;
		AvailabilityResult.SlotAvailabilities.Emplace(
			FInv_SlotAvailability{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftIndex() : GridSlot->GetTileIndex(),
				AvailabilityResult.bStackable ? AmountToFillSlot : 0,
				HasValidItem(GridSlot)
			}
		);

		AmountToFill -= AmountToFillSlot;
		AvailabilityResult.Remainder = AmountToFill;
		if (AmountToFill == 0)
		{
			return AvailabilityResult;
		}
	}
	
	return AvailabilityResult;
}

FIntPoint UInv_InventoryGrid::GetItemDimensions(const FInv_ItemManifest& Manifest) const
{
	const FInv_GridFragment* GridFragment = Manifest.GetFragmentOfType<FInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
}

bool UInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	if (StartIndex < 0 || StartIndex >= GridSlots.Num())
	{
		return false;
	}
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

int32 UInv_InventoryGrid::DetermineFillAmountForSlot(
	const bool bStackable,
	const int32 MaxStackSize,
	const int32 AmountToFill,
	const UInv_GridSlot* GridSlot) const
{
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 UInv_InventoryGrid::GetStackAmount(const UInv_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackAmount = GridSlot->GetStackCount();
	if (const int32 UpperLeftIndex = GridSlot->GetUpperLeftIndex(); UpperLeftIndex != INDEX_NONE)
	{
		UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
		CurrentSlotStackAmount = UpperLeftGridSlot->GetStackCount();
	}
	return CurrentSlotStackAmount;
}

void UInv_InventoryGrid::AddStacks(const FInv_SlotAvailabilityResult& AvailabilityResult)
{
	if (!MatchesCategory(AvailabilityResult.Item.Get()))
	{
		return;
	}
	for (const auto& Availability : AvailabilityResult.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex)
		{
			const auto& GridSlot = GridSlots[Availability.Index];
			const auto& SlottedItem = SlottedItems.FindChecked(Availability.Index);
			int32 NewStackAmount = GridSlot->GetStackCount() + Availability.AmountToFill;
			SlottedItem->UpdateStackCount(NewStackAmount);
			GridSlot->SetStackCount(NewStackAmount);
		}
		else
		{
			AddItemAtIndex(AvailabilityResult.Item.Get(), Availability.Index, AvailabilityResult.bStackable, Availability.AmountToFill);
			UpdateGridSlots(AvailabilityResult.Item.Get(), Availability.Index, AvailabilityResult.bStackable, Availability.AmountToFill);
		}
	}
	
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
	if (!MatchesCategory(InItem))
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
	SlottedItem->OnSlottedItemClickedDelegate.AddDynamic(this, &ThisClass::OnSlottedItemClicked);
	
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

			const FIntPoint TilePosition(j, i);
			GridSlot->SetTileIndex(UInv_WidgetUtils::GetIndexFromPosition(TilePosition, Columns));

			UCanvasPanelSlot* GridCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCanvasPanelSlot->SetSize(FVector2d(TileSize));
			GridCanvasPanelSlot->SetPosition(TilePosition * TileSize);

			GridSlots.Add(GridSlot);
			GridSlot->OnGridSlotClickedDelegate.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->OnGridSlotHoveredDelegate.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->OnGridSlotUnhoveredDelegate.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);
		}
	}
}

void UInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem) || !GridSlots.IsValidIndex(ItemDropIndex))
	{
		return;
	}
	if (CurrentQueryResult.ValidItem.IsValid() && GridSlots.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex, MouseEvent);
		return;
	}

	if (!IsInGridBounds(ItemDropIndex, HoverItem->GetGridDimension()))
	{
		return;
	}
	
	auto GridSlot = GridSlots[ItemDropIndex];
	if (!GridSlot->GetInventoryItem().IsValid())
	{
		PutDownIndex(ItemDropIndex);
	}
}

void UInv_InventoryGrid::PutDownIndex(const int32 Index)
{
	AddItemAtIndex(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	UpdateGridSlots(HoverItem->GetInventoryItem(), Index, HoverItem->IsStackable(), HoverItem->GetStackCount());
	ClearHoverItem();
}

void UInv_InventoryGrid::ClearHoverItem()
{
	if (!IsValid(HoverItem))
	{
		return;
	}
	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());

	HoverItem->RemoveFromParent();
	HoverItem = nullptr;

	ShowCursor();
}

UUserWidget* UInv_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer()))
	{
		return nullptr;
	}
	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}
	return VisibleCursorWidget;
}

UUserWidget* UInv_InventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer()))
	{
		return nullptr;
	}
	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}
	return HiddenCursorWidget;
}

void UInv_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer()))
	{
		return;
	}
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void UInv_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer()))
	{
		return;
	}
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());
}

bool UInv_InventoryGrid::HasHoverItem() const
{
	return IsValid(HoverItem);
}

UInv_HoverItem* UInv_InventoryGrid::GetHoverItem() const
{
	if (!IsValid(HoverItem))
	{
		return nullptr;
	}
	
	return HoverItem;
}

void UInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem))
	{
		return;
	}

	UInv_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->GetAvailable())
	{
		GridSlot->SetOccupiedTexture();
	}
}

void UInv_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem))
	{
		return;
	}
	UInv_GridSlot* GridSlot = GridSlots[GridIndex];
	if (GridSlot->GetAvailable())
	{
		GridSlot->SetUnoccupiedTexture();
	}
}

bool UInv_InventoryGrid::MatchesCategory(const UInv_InventoryItem* InItem) const
{
	return InItem->GetItemManifest().GetItemCategory() == ItemCategory;
}

void UInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	UInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());
	check(GridSlots.IsValidIndex(GridIndex));
	UInv_InventoryItem* ClickedInventoryItem = GridSlots[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(HoverItem) && IsLeftClick(MouseEvent))
	{
		PickUp(ClickedInventoryItem, GridIndex);
		return;
	}

	if (IsRightClick(MouseEvent))
	{
		CreateItemPopUp(GridIndex);
		return;
	}

	if (IsSameStackable(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlots[GridIndex]->GetStackCount();
		const FInv_StackableFragment* StackableFragment = ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FInv_StackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 HoveredStackCount = HoverItem->GetStackCount();
		
		if (ShouldSwapStackCount(RoomInClickedSlot, HoveredStackCount, MaxStackSize))
		{
			SwapStackCount(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		if (ShouldConsumeHoverItemStacks(HoveredStackCount, RoomInClickedSlot))
		{
			ConsumeHoverItemStacks(ClickedStackCount, HoveredStackCount, GridIndex);
			return;
		}
		if (ShouldFillInStack(RoomInClickedSlot, HoveredStackCount))
		{
			FillInStack(RoomInClickedSlot, HoveredStackCount - RoomInClickedSlot, GridIndex);
			return;
		}

		if (RoomInClickedSlot == 0)
		{
			// TODO: Add something - for example sound
			return;
		}
		return;
	}

	if (CurrentQueryResult.ValidItem.IsValid())
	{
		SwapWithHoverItem(ClickedInventoryItem, GridIndex);
	}
}

void UInv_InventoryGrid::CreateItemPopUp(const int32 GridIndex)
{
	UInv_InventoryItem* RightClickedItem = GridSlots[GridIndex]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem))
	{
		return;
	}
	if (IsValid(GridSlots[GridIndex]->GetItemPopUp()))
	{
		return;
	}

	ItemPopUpWidget = CreateWidget<UInv_ItemPopUp>(GetOwningPlayer(), ItemPopUpClass);
	GridSlots[GridIndex]->SetItemPopUp(ItemPopUpWidget);
	
	OwningCanvasPanel->AddChild(ItemPopUpWidget);
	UCanvasPanelSlot* CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemPopUpWidget);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	CanvasPanelSlot->SetPosition(MousePosition - ItemPopUpOffset);
	CanvasPanelSlot->SetSize(ItemPopUpWidget->GetBoxSize());

	const int32 SliderMax = GridSlots[GridIndex]->GetStackCount() - 1;
	if (RightClickedItem->IsStackable() && SliderMax > 0)
	{
		ItemPopUpWidget->OnPopUpMenuSplitDelegate.BindDynamic(this, &ThisClass::OnPopUpMenuSplit);
		ItemPopUpWidget->SetSliderParams(SliderMax, FMath::Max(1, GridSlots[GridIndex]->GetStackCount() /2));
	}
	else
	{
		ItemPopUpWidget->CollapseSplitButton();
	}

	ItemPopUpWidget->OnPopUpMenuDropDelegate.BindDynamic(this, &ThisClass::OnPopUpMenuDrop);

	if (RightClickedItem->IsConsumable())
	{
		ItemPopUpWidget->OnPopUpMenuConsumeDelegate.BindDynamic(this, &ThisClass::OnPopUpMenuConsume);
	}
	else
	{
		ItemPopUpWidget->CollapseConsumeButton();
	}
}

void UInv_InventoryGrid::DropItem(const FIntPoint InMousePosition)
{
	if (!IsValid(HoverItem) || !IsValid(HoverItem->GetInventoryItem()))
	{
		return;
	}

	InventoryComponent->Server_DropItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount(), InMousePosition);

	ClearHoverItem();
	ShowCursor();
}

void UInv_InventoryGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	UInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem) || !RightClickedItem->IsStackable())
	{
		return;
	}

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItems.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	AssignHoverItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void UInv_InventoryGrid::OnPopUpMenuDrop(int32 Index)
{
	UInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem))
	{
		return;
	}

	PickUp(RightClickedItem, Index);
	DropItem(FIntPoint(INT_MAX, INT_MAX));
}

void UInv_InventoryGrid::OnPopUpMenuConsume(int32 Index)
{
	UInv_InventoryItem* RightClickedItem = GridSlots[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem))
	{
		return;
	}

	const int32 UpperLeftIndex = GridSlots[Index]->GetUpperLeftIndex();
	UInv_GridSlot* UpperLeftGridSlot = GridSlots[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - 1;

	UpperLeftGridSlot->SetStackCount(NewStackCount);
	SlottedItems.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	InventoryComponent->Server_ConsumeItem(RightClickedItem);

	if (NewStackCount == 0)
	{
		RemoveItemFromGrid(RightClickedItem, Index);
	}
}

void UInv_InventoryGrid::OnInventoryMenuToggled(bool bOpen)
{
	if (!bOpen)
	{
		PutHoverItemBack();
	}
}

bool UInv_InventoryGrid::IsSameStackable(const UInv_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();
	return bIsSameItem && bIsStackable && HoverItem->GetItemType().MatchesTagExact(
		ClickedInventoryItem->GetItemManifest().GetItemType());
}

void UInv_InventoryGrid::SwapWithHoverItem(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(HoverItem))
	{
		return;
	}

	UInv_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();

	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemAtIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

bool UInv_InventoryGrid::ShouldSwapStackCount(const int32 RoomInClickedSlot, const int32 HoveredStackCount,
	const int32 MaxStackSize) const
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void UInv_InventoryGrid::SwapStackCount(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index) const
{
	UInv_GridSlot* Grid = GridSlots[Index];
	GridSlots[Index]->SetStackCount(HoveredStackCount);

	UInv_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(HoveredStackCount);

	HoverItem->UpdateStackCount(ClickedStackCount);
}

bool UInv_InventoryGrid::ShouldConsumeHoverItemStacks(const int32 HoveredStackCount,
	const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void UInv_InventoryGrid::ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;

	GridSlots[Index]->SetStackCount(NewClickedStackCount);
	SlottedItems.FindChecked(Index)->UpdateStackCount(NewClickedStackCount);
	ClearHoverItem();
	ShowCursor();

	const FInv_GridFragment* GridFragment = GridSlots[Index]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FInv_GridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
	HighlightSlots(Index, Dimensions);
}

bool UInv_InventoryGrid::ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const
{
	return RoomInClickedSlot < HoveredStackCount;
}

void UInv_InventoryGrid::FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	UInv_GridSlot* GridSlot = GridSlots[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

	GridSlot->SetStackCount(NewStackCount);

	UInv_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(NewStackCount);

	HoverItem->UpdateStackCount(Remainder);
}

void UInv_InventoryGrid::PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	AssignHoverItem(ClickedInventoryItem, GridIndex, GridIndex);
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
}

void UInv_InventoryGrid::AssignHoverItem(
	UInv_InventoryItem* InInventoryItem,
	const int32 GridIndex,
	const int32 PreviousGridIndex)
{
	AssignHoverItem(InInventoryItem);

	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
	HoverItem->UpdateStackCount(InInventoryItem->IsStackable() ? GridSlots[GridIndex]->GetStackCount() : 0);
}

void UInv_InventoryGrid::RemoveItemFromGrid(UInv_InventoryItem* InInventoryItem, const int32 GridIndex)
{
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(
			InInventoryItem, FInventoryFragmentsTags::Get().GridFragment);
	if (!GridFragment)
	{
		return;
	}

	UInv_InventoryStatics::ForEach2D(GridSlots, GridIndex, GridFragment->GetGridSize(), Columns,
		[&](UInv_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftIndex(INDEX_NONE);
			GridSlot->SetUnoccupiedTexture();
			GridSlot->SetAvailable(true);
			GridSlot->SetStackCount(0);
		});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<UInv_SlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

void UInv_InventoryGrid::AssignHoverItem(UInv_InventoryItem* InInventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<UInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}
	
	const FInv_GridFragment* GridFragment = GetFragment<FInv_GridFragment>(
			InInventoryItem, FInventoryFragmentsTags::Get().GridFragment);
	const FInv_ImageFragment* ImageFragment = GetFragment<FInv_ImageFragment>(
		InInventoryItem, FInventoryFragmentsTags::Get().IconFragment);
	if (!GridFragment || !ImageFragment)
	{
		return;
	}
	
	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimension(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InInventoryItem);
	HoverItem->SetIsStackable(InInventoryItem->IsStackable());
	
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, HoverItem);
}

void UInv_InventoryGrid::PutHoverItemBack()
{
	if (!IsValid(HoverItem))
	{
		return;
	}

	FInv_SlotAvailabilityResult AvailabilityResult = HasRoomForItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount());
	AvailabilityResult.Item = HoverItem->GetInventoryItem();

	AddStacks(AvailabilityResult);
	ClearHoverItem();
}

void UInv_InventoryGrid::OnHide()
{
	PutHoverItemBack();
}

bool UInv_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UInv_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}
