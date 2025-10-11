// Copyright - none


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "ContentBrowserDataSource.h"
#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/GridSlots/Inv_EquippedGridSlot.h"
#include "Widgets/Inventory/HoverItem/Inv_HoverItem.h"
#include "Widgets/Inventory/ItemDescription/Inv_ItemDescription.h"
#include "Widgets/Inventory/SlottedItems/Inv_EquippedSlottedItem.h"

void UInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button_Equippables->OnClicked.AddDynamic(this, &ThisClass::ShowEquippables);
	Button_Consumables->OnClicked.AddDynamic(this, &ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this, &ThisClass::ShowCraftables);

	Grid_Equippables->SetOwningCanvasPanel(CanvasPanel);
	Grid_Consumables->SetOwningCanvasPanel(CanvasPanel);
	Grid_Craftables->SetOwningCanvasPanel(CanvasPanel);

	ShowEquippables();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		if (UInv_EquippedGridSlot* EquippedGridSlot = Cast<UInv_EquippedGridSlot>(Widget); IsValid(EquippedGridSlot))
		{
			EquippedGridSlots.Add(EquippedGridSlot);
			EquippedGridSlot->OnEquippedGridSlotClickedDelegate.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
		}
	});
}

void UInv_SpatialInventory::EquippedGridSlotClicked(
	UInv_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquippedTypeTag)
{
	if (!CanEquipHoverItem(EquippedGridSlot, EquippedTypeTag))
	{
		return;
	}
	const UInv_HoverItem* HoverItem = GetHoverItem();
	const float TileSize = UInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetTileSize();
	UInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
		HoverItem->GetInventoryItem(),
		EquippedTypeTag,
		TileSize);

	EquippedSlottedItem->OnClickedSlottedItemClickedDelegate.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);

	Grid_Equippables->ClearHoverItem();

	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(), nullptr);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipDelegate.Broadcast(HoverItem->GetInventoryItem());
	}
	// Work here
}

void UInv_SpatialInventory::EquippedSlottedItemClicked(UInv_EquippedSlottedItem* EquippedSlottedItem)
{
}

bool UInv_SpatialInventory::CanEquipHoverItem(
	const UInv_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquippedTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid())
	{
		return false;
	}

	UInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem))
	{
		return false;
	}

	UInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	bool bHasHover = HasHoverItem();
	bool bHeldCheck = IsValid(HeldItem)
					&& !HoverItem->IsStackable()
					&& HeldItem->GetItemManifest().GetItemCategory() == EInv_ItemCategory::Equippable
					&& HeldItem->GetItemManifest().GetItemType().MatchesTag(EquippedTypeTag);
	
	return bHasHover && bHeldCheck;
}


void UInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ItemDescriptionWidget)
	{
		return;
	}
	SetItemDescriptionSizeAnPosition(ItemDescriptionWidget, CanvasPanel);
}

void UInv_SpatialInventory::SetItemDescriptionSizeAnPosition(
	UInv_ItemDescription* ItemDescription,
	UCanvasPanel* InCanvasPanel) const
{
	UCanvasPanelSlot* ItemDescriptionCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(ItemDescriptionWidget);
	if (!IsValid(ItemDescriptionCanvasPanelSlot))
	{
		return;
	}

	const FVector2D ItemsDescriptionSize = ItemDescription->GetBoxSize();
	ItemDescriptionCanvasPanelSlot->SetSize(ItemsDescriptionSize);

	FVector2D CanvasPanelSize = UInv_WidgetUtils::GetWidgetSize(InCanvasPanel);
	FVector2D ClampedPosition = UInv_WidgetUtils::GetClampedWidgetPosition(
		CanvasPanelSize,
		ItemsDescriptionSize,
		UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));

	ItemDescriptionCanvasPanelSlot->SetPosition(ClampedPosition);
}

FReply UInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                      const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

void UInv_SpatialInventory::OnItemHovered(UInv_InventoryItem* ItemsComponent)
{
	const FInv_ItemManifest& Manifest = ItemsComponent->GetItemManifest();
	UInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda(
		[this, &Manifest, DescriptionWidget]()
		{
			Manifest.AssimilateInventoryFragments(DescriptionWidget);
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		});
	
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(
		DescriptionTimer, DescriptionTimerDelegate, DescriptionTimerDelay, false);
}

void UInv_SpatialInventory::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool UInv_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem() || Grid_Consumables->HasHoverItem() || Grid_Craftables->HasHoverItem())
	{
		return true;
	}
	return false;
}

UInv_HoverItem* UInv_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid())
	{
		return nullptr;
	}

	return ActiveGrid->GetHoverItem();
}

float UInv_SpatialInventory::GetTileSize() const
{
	return Grid_Equippables->GetTileSize();
}

FInv_SlotAvailabilityResult UInv_SpatialInventory::HasRoomForItem(UInv_ItemComponent* ItemComponent) const
{
	switch (UInv_InventoryStatics::GetItemCategoryFromItemComponent(ItemComponent))
	{
		case EInv_ItemCategory::Equippable:
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Consumable:
			return Grid_Consumables->HasRoomForItem(ItemComponent);
		case EInv_ItemCategory::Craftable:
			return Grid_Craftables->HasRoomForItem(ItemComponent);
		default:
			{
				UE_LOG(LogInventory, Error, TEXT("ItemComponent doesn't have a valid Item Category."));
				return FInv_SlotAvailabilityResult();
			}
	}
}

void UInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void UInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void UInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void UInv_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

void UInv_SpatialInventory::SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button)
{
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->HideCursor();
	}
	ActiveGrid = Grid;
	if (ActiveGrid.IsValid())
	{
		ActiveGrid->ShowCursor();
	}
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

UInv_ItemDescription* UInv_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescriptionWidget))
	{
		ItemDescriptionWidget = CreateWidget<UInv_ItemDescription>(GetOwningPlayer(), ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescriptionWidget);
	}
	return ItemDescriptionWidget;
}
