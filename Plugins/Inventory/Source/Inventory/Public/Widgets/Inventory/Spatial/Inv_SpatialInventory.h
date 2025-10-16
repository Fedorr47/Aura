// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/Button.h"
#include "Items/Inv_InventoryItem.h"
#include "Widgets/Inventory/InventoryBase/Inv_InventoryBase.h"
#include "Inv_SpatialInventory.generated.h"

class UInv_EquippedSlottedItem;
class UInv_EquippedGridSlot;
class UInv_ItemDescription;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class UInv_InventoryGrid;
class UInv_HoverItem;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_SpatialInventory : public UInv_InventoryBase
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FInv_SlotAvailabilityResult HasRoomForItem(UInv_ItemComponent* ItemComponent) const;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void OnItemHovered(UInv_InventoryItem* Item) override;
	virtual void OnItemUnhovered()override;
	virtual bool HasHoverItem() const override;
	virtual UInv_HoverItem* GetHoverItem() const override;
	virtual float GetTileSize() const override;
	
private:
	
	UFUNCTION()
	void ShowEquippables();
	UFUNCTION()
	void ShowConsumables();
	UFUNCTION()
	void ShowCraftables();
	UFUNCTION()
	void EquippedGridSlotClicked( UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquippedTypeTag);
	UFUNCTION()
	void EquippedSlottedItemClicked(UInv_EquippedSlottedItem* EquippedSlottedItem);
	UFUNCTION()
	void ShowEquippedItemDescription(UInv_InventoryItem* InventoryItem);
	
	void DisableButton(UButton* Button);
	void SetActiveGrid(UInv_InventoryGrid* Grid, UButton* Button);
	UInv_ItemDescription* GetItemDescription();
	UInv_ItemDescription* GetEquippedItemDescription();
	void SetItemDescriptionSizeAnPosition(UInv_ItemDescription* ItemDescription, UCanvasPanel* InCanvasPanel) const;
	void SetEquippedItemDescriptionSizeAnPosition(
		UInv_ItemDescription* ItemDescription,
		UInv_ItemDescription* EquippedItemDescription,
		UCanvasPanel* InCanvasPanel) const;
	bool CanEquipHoverItem(const UInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquippedTypeTag) const;
	UInv_EquippedGridSlot* FindSlotWithEquippedItem(UInv_InventoryItem* EquippedItem) const;
	void ClearSlotOfItem(UInv_EquippedGridSlot* EquippedGridSlot);
	void RemoveEquippedSlottedItem(UInv_EquippedSlottedItem* InEquippedSlottedItem);
	void MakeEquippedSlottedItem(
		UInv_EquippedSlottedItem* InEquippedSlottedItem,
		UInv_EquippedGridSlot* InEquippedGridSlot,
		UInv_InventoryItem* InItemToEquip);
	void BroadcastSlotClickedDelegates(UInv_InventoryItem* InItemToEquip, UInv_InventoryItem* InItemToUnequip) const;

//--------------------------------------------------------------------------------------------------------------------//

	UPROPERTY()
	TArray<TObjectPtr<UInv_EquippedGridSlot>> EquippedGridSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInv_InventoryGrid> Grid_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	TWeakObjectPtr<UInv_InventoryGrid> ActiveGrid;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UInv_ItemDescription> ItemDescriptionWidget;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TSubclassOf<UInv_ItemDescription> EquippedItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<UInv_ItemDescription> EquippedItemDescriptionWidget;

	FTimerHandle DescriptionTimer;
	FTimerHandle EquippedDescriptionTimer;
	
	UPROPERTY(EditAnywhere, Category = Inventory)
	float DescriptionTimerDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category = Inventory)
	float EquippedDescriptionTimerDelay = 0.5f;
};
