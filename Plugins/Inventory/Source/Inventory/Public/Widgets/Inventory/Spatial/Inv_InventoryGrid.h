// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


struct FInv_GridFragment;
struct FInv_ImageFragment;
class UInv_SlottedItem;
class UCanvasPanel;
class UInv_GridSlot;
class UInv_InventoryComponent;

UCLASS()
class INVENTORY_API UInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	EInv_ItemCategory GetItemCategory() const {return ItemCategory; }
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* InItem);
	

private:

	void ConstructGrid();

	bool MatchCategory(const UInv_InventoryItem* InItem) const;

	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_InventoryItem* Item);
	FInv_SlotAvailabilityResult HasRoomForItem(const FInv_ItemManifest& Manifest);
	void AddItemToIndecies(const FInv_SlotAvailabilityResult& AvailabilityResult, UInv_InventoryItem* InItem);
	FVector2D GetDrawSize(const FInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(
		const UInv_SlottedItem* SlottedItem,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(
		UInv_InventoryItem* InItem,
		const int32 Index,
		const bool bStackable,
		const int32 StackAmount);
	UInv_SlottedItem* CreateSlottedItem(
		UInv_InventoryItem* InItem,
		const bool bStackable,
		const int32 StackAmount,
		const FInv_GridFragment* GridFragment,
		const FInv_ImageFragment* ImageFragment,
		const int32 Index) const;
	void AddSlottedItemToCanvas(
		const int32 Index,
		const FInv_GridFragment* GridFragment,
		UInv_SlottedItem* SlottedItem) const;
	void UpdateGridSlots(
		UInv_InventoryItem* InItem,
		const int32 Index,
		bool bStackable,
		const int32 StackAmount);
	bool IsIndexClaimed(const TSet<int32>& CheckeIndecies, const int32 Index) const;
	bool HasRoomAtIndex(
		const UInv_GridSlot* GridSlot,
		const FIntPoint& Dimensions,
		TSet<int32>& CheckeIndecies,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize);
	FIntPoint GetItemDimensions(const FInv_ItemManifest& Manifest) const;
	bool CheckSlotConstraints(
		const UInv_GridSlot* GridSlot,
		const UInv_GridSlot* SubGridSlot,
		const TSet<int32>& CheckedIndecies,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize) const;
	bool HasValidItem(const UInv_GridSlot* SubGridSlot) const;
	bool IsUpperLeftSlot(
		const UInv_GridSlot* GridSlot,
		const UInv_GridSlot* SubGridSlot) const;
	bool DoesItemMatch(
		const UInv_InventoryItem* SubItem,
		const FGameplayTag& ItemType) const;

	//------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	EInv_ItemCategory ItemCategory;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<UInv_SlottedItem>> SlottedItems;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Rows{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Columns{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float TileSize{1};
};
