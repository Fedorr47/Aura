// Copyright - none


#include "Widgets/Inventory/Spatial/Inv_SpatialInventory.h"

#include "ContentBrowserDataSource.h"
#include "Inventory.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Widgets/Inventory/Spatial/Inv_InventoryGrid.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Widgets/Inventory/ItemDescription/Inv_ItemDescription.h"

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
	UInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);

	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda(
		[this]()
		{
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
