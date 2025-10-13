// Copyright - none


#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

#include "Blueprint/UserWidget.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Internal/VT/VirtualTextureVisualizationData.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult AvailabilityResult = InventoryMenuWidget->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	AvailabilityResult.Item = FoundItem;

	if (AvailabilityResult.TotalRoomToFill == 0)
	{
		OnNoRoomInventoryDelegate.Broadcast();
		return;
	}

	if (AvailabilityResult.Item.IsValid() && AvailabilityResult.bStackable)
	{
		OnStackChangedDelegate.Broadcast(AvailabilityResult);
		Server_AddStackToItem(
			ItemComponent,
			AvailabilityResult.TotalRoomToFill,
			AvailabilityResult.Remainder);
	}
	else if (AvailabilityResult.TotalRoomToFill > 0)
	{
		Server_AddNewItem(
			ItemComponent,
			AvailabilityResult.bStackable ? AvailabilityResult.TotalRoomToFill : 0);
	}
}

void UInv_InventoryComponent::Server_AddNewItem_Implementation(
	UInv_ItemComponent* ItemsComponent,
	int32 StackCount)
{
	UInv_InventoryItem* NewItem =  InventoryList.AddEntry(ItemsComponent);
	NewItem->SetTotalStackCount(StackCount);

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAddedDelegate.Broadcast(NewItem);
	}

	ItemsComponent->PickedUp();
}

void UInv_InventoryComponent::Server_AddStackToItem_Implementation(
	UInv_ItemComponent* ItemComponent,
	int32 StackCount,
	int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ?
		ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	UInv_InventoryItem* Item =  InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item))
	{
		return;
	}

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	else if (FInv_StackableFragment* StackableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void UInv_InventoryComponent::Server_DropItem_Implementation(
	UInv_InventoryItem* InventoryItem,
	int32 StackCount,
	const FIntPoint MousePosition)
{
	const int32 NewStackCount = InventoryItem->GetTotalStackCount() - StackCount;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveEntry(InventoryItem);
	}
	else
	{
		InventoryItem->SetTotalStackCount(NewStackCount);
	}

	SpawnDroppedItem(InventoryItem, StackCount, MousePosition);
}

void UInv_InventoryComponent::SpawnDroppedItem(
	UInv_InventoryItem* InventoryItem,
	int32 StackCount,
	const FIntPoint MousePosition)
{
	const APawn* OwnningPawn = OwningController->GetPawn();
	FVector RotatedForward = OwnningPawn->GetActorForwardVector();
	RotatedForward =
		RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngMin,DropSpawnAngMax), FVector::UpVector);
	FVector SpawnLocation;
	if (FMath::Abs(MousePosition.X) < INT_MAX && FMath::Abs(MousePosition.Y) < INT_MAX)
	{
		SpawnLocation = FVector(MousePosition.X, MousePosition.Y, RelativeSpawnElevation);
	}
	else
	{
		SpawnLocation =	OwnningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin, DropSpawnDistanceMax);
		SpawnLocation.Z -= RelativeSpawnElevation;
	}
	
	const FRotator SpawnRotation = FRotator::ZeroRotator;

	FInv_ItemManifest& ItemManifest = InventoryItem->GetItemManifestMutable();
	if (FInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}
	ItemManifest.SpawnPickupActor(this, SpawnLocation, SpawnRotation);
}

void UInv_InventoryComponent::Server_ConsumeItem_Implementation(UInv_InventoryItem* InventoryItem)
{
	const int32 NewStackCount = InventoryItem->GetTotalStackCount() - 1;
	if (NewStackCount == 0)
	{
		InventoryList.RemoveEntry(InventoryItem);
	}
	else
	{
		InventoryItem->SetTotalStackCount(NewStackCount);
	}

	if (FInv_ConsumableFragment* ConsumableFragment = InventoryItem->GetItemManifestMutable().GetFragmentOfTypeMutable<FInv_ConsumableFragment>())
	{
		ConsumableFragment->OnConsume(this);
	}
}

void UInv_InventoryComponent::Server_EquipSlotClicked_Implementation(UInv_InventoryItem* InventoryItemToEquip,
	UInv_InventoryItem* EquippedSlotToUnequip)
{
	Multicast_EquipSlotClicked(InventoryItemToEquip, EquippedSlotToUnequip);
}

void UInv_InventoryComponent::Multicast_EquipSlotClicked_Implementation(UInv_InventoryItem* InventoryItemToEquip,
	UInv_InventoryItem* InventoryItemToUnequip)
{
	OnItemEquipDelegate.Broadcast(InventoryItemToEquip);
	OnItemEquipDelegate.Broadcast(InventoryItemToUnequip);
}

void UInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
	OnInventoryMenuToggledDelegate.Broadcast(bInventoryMenuOpen);
}

void UInv_InventoryComponent::AddRepSubObject(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory component should have a PlayerController as Owner"));
	if (!OwningController->IsLocalController())
	{
		return;
	}

	InventoryMenuWidget = CreateWidget<UInv_InventoryBase>(
		OwningController.Get(), InventoryMenuWidgetClass);
	InventoryMenuWidget->AddToViewport();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenuWidget))
	{
		return;
	}

	InventoryMenuWidget->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid())
	{
		return;
	}

	/*
	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
	*/
}

void UInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenuWidget))
	{
		return;
	}

	InventoryMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryMenuOpen = false;

	if (!OwningController.IsValid())
	{
		return;
	}

	/*
	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
	*/

	/* For top down game*/
	if (!OwningController.IsValid())
	{
		return;
	}
	OwningController.Get()->SetMouseCursorWidget(EMouseCursor::Default, GeDefaultCursorWidget());
}

// TODO: Move it to game code itself
UUserWidget* UInv_InventoryComponent::GeDefaultCursorWidget()
{
	if (!OwningController.IsValid())
	{
		return nullptr;
	}
	if (!IsValid(DefaultCursorWidget))
	{
		DefaultCursorWidget = CreateWidget<UUserWidget>(OwningController.Get(), DefaultCursorWidgetClass);
	}
	return DefaultCursorWidget;
}


