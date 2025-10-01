// Copyright - none


#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

#include "Blueprint/UserWidget.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Net/UnrealNetwork.h"

UInv_InventoryComponent::UInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void UInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

void UInv_InventoryComponent::TryAddItem(UInv_ItemComponent* ItemComponent)
{
	FInv_SlotAvailabilityResult AvailabilityResult = InventoryMenuObj->HasRoomForItem(ItemComponent);

	UInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	AvailabilityResult.Item = FoundItem;

	if (AvailabilityResult.TotalRoomToFill == 0)
	{
		OnNoRoomInventoryDelegate.Broadcast();
		return;
	}

	if (AvailabilityResult.Item.IsValid() && AvailabilityResult.bStackable)
	{
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

	if (GetOwner()->GetNetMode() == NM_ListenServer || GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAddedDelegate.Broadcast(NewItem);
	}
	// TODO: Destroy item
}

void UInv_InventoryComponent::Server_AddStackToItem_Implementation(
	UInv_ItemComponent* ItemsComponent,
	int32 StackCount,
	int32 Remainder)
{
	
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
}

void UInv_InventoryComponent::AddRepSubObject(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

// Called when the game starts
void UInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ConstructInventory();
}

void UInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());
	checkf(OwningController.IsValid(), TEXT("Inventory component should have a PlayerController as Owner"));
	if (!OwningController->IsLocalController())
	{
		return;
	}

	InventoryMenuObj = CreateWidget<UInv_InventoryBase>(
		OwningController.Get(), InventoryMenuClass);
	InventoryMenuObj->AddToViewport();
	CloseInventoryMenu();
}

void UInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenuObj))
	{
		return;
	}

	InventoryMenuObj->SetVisibility(ESlateVisibility::Visible);
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
	if (!IsValid(InventoryMenuObj))
	{
		return;
	}

	InventoryMenuObj->SetVisibility(ESlateVisibility::Collapsed);
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
}


