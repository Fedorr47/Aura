// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_InventoryBase.generated.h"


class UInv_HoverItem;
class UInv_ItemComponent;

UCLASS()
class INVENTORY_API UInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemsComponent) const
	{ return FInv_SlotAvailabilityResult(); }

	virtual void OnItemHovered(UInv_InventoryItem* ItemComponent){}
	virtual void OnItemUnhovered(){}
	virtual bool HasHoverItem() const { return false; }
	virtual UInv_HoverItem* GetHoverItem() const { return nullptr; }
	virtual float GetTileSize() const { return 0.0f; }
};
