// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryBase.generated.h"


class UInv_DisplayItemsComponent;

UCLASS()
class INVENTORY_API UInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_DisplayItemsComponent* ItemsComponent) const
	{ return FInv_SlotAvailabilityResult(); }
};
