// Copyright - none

#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Widgets/Inventory/Inv_GridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"

void UInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ConstructGrid();

	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &ThisClass::AddItem);
}

void UInv_InventoryGrid::AddItem(UInv_InventoryItem* InItem)
{
	if (!MatchCategory(InItem))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem"));
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
