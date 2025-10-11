// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlot.h"
#include "Components/Overlay.h"
#include "Inv_EquippedGridSlot.generated.h"

class UInv_EquippedSlottedItem;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClickedSignature, UInv_EquippedGridSlot*, GridSloot, const FGameplayTag&, EquipmentTypeTag);

UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UInv_EquippedSlottedItem* OnItemEquipped(
		UInv_InventoryItem* InInventoryItem,
		const FGameplayTag& InEquippedTypeTag,
		float TileSize);
	//----------------------------------------------------------------------------------------------------------------//
	
	FEquippedGridSlotClickedSignature OnEquippedGridSlotClickedDelegate;
	
private:

	//----------------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, Category = Inventory, meta = (Categories = "Inventory.GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ImageGrayOutIcon;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_EquippedSlottedItem> EquippedSlottedItemClass;

	UPROPERTY()
	TObjectPtr<UInv_EquippedSlottedItem> EquippedSlottedItemWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> OverlayRoot;
};
