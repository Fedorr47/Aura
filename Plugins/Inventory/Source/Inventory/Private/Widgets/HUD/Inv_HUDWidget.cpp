// Copyright - none


#include "Widgets/HUD/Inv_HUDWidget.h"

#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Widgets/HUD/Inv_InfoMessage.h"

void UInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInv_InventoryComponent* InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->OnNoRoomInventoryDelegate.AddDynamic(this, &ThisClass::OnNoRoom);
	}
}

void UInv_HUDWidget::OnNoRoom()
{
	if (!IsValid(InfoMessage))
	{
		return;	
	}
	InfoMessage->SetMessage(FText::FromString("No Room In Inventory"));
}
