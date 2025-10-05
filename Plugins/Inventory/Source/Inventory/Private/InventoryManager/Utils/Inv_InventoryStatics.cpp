// Copyright - none


#include "InventoryManager/Utils/Inv_InventoryStatics.h"

#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"

UInv_InventoryComponent* UInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}

	UInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<UInv_InventoryComponent>();
	return InventoryComponent;
}

bool UInv_InventoryStatics::HasValidInventoryBaseWidget(
	const APlayerController* PlayerController,
	UInv_InventoryBase*& InventoryBaseWidget)
{
	UInv_InventoryComponent* InventoryComponent = GetInventoryComponent(PlayerController);
	if (!IsValid(InventoryComponent))
	{
		return false;
	}

	InventoryBaseWidget = InventoryComponent->GetInventoryWidget();
	if (!IsValid(InventoryBaseWidget))
	{
		return false;
	}
	return true;
}

void UInv_InventoryStatics::ItemHovered(
	APlayerController* PlayerController,
	UInv_InventoryItem* InventoryItem)
{
	UInv_InventoryBase* InventoryBaseWidget;
	if (!HasValidInventoryBaseWidget(PlayerController, InventoryBaseWidget)) return;

	if (InventoryBaseWidget->HasHoverItem())
	{
		return;
	}

	InventoryBaseWidget->OnItemHovered(InventoryItem);
}

void UInv_InventoryStatics::ItemUnhovered(APlayerController* PlayerController)
{
	UInv_InventoryBase* InventoryBaseWidget;
	if (!HasValidInventoryBaseWidget(PlayerController, InventoryBaseWidget)) return;

	InventoryBaseWidget->OnItemUnhovered();
}
