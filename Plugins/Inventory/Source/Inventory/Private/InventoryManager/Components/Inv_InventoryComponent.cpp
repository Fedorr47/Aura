// Copyright - none


#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

#include "Blueprint/UserWidget.h"
#include "Items/Inv_DisplayItemsComponent.h"


UInv_InventoryComponent::UInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInv_InventoryComponent::TryAddItem(UInv_DisplayItemsComponent* ItemComponent)
{
	OnNoRoomInventoryDelegate.Broadcast();
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


