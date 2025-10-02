// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


enum class EInv_GridSlotState : uint8;
class UInv_HoverItem;
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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;

	EInv_ItemCategory GetItemCategory() const {return ItemCategory; }
	FInv_SlotAvailabilityResult HasRoomForItem(const UInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(UInv_InventoryItem* InItem);
	

private:

	void ConstructGrid();

	bool MatchesCategory(const UInv_InventoryItem* InItem) const;

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
	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	int32 DetermineFillAmountForSlot(
		const bool bStackable,
		const int32 MaxStackSize,
		const int32 AmountToFill,
		const UInv_GridSlot* GridSlot) const;
	int32 GetStackAmount(const UInv_GridSlot* GridSlot) const;

	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;


	UFUNCTION()
	void AddStacks(const FInv_SlotAvailabilityResult& AvailabilityResult);
	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);
	void PickUp(UInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	void AssignHoverItem(UInv_InventoryItem* InInventoryItem);
	void AssignHoverItem(UInv_InventoryItem* InInventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);
	void RemoveItemFromGrid(UInv_InventoryItem* InInventoryItem, const int32 GridIndex);
	void UpdateTileParameters(const FVector2D& CanvasPosition, const FVector2D& MousePosition);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	EInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPosition, const FVector2D& MousePosition) const;
	void OnTileParametersUpdated(const FInv_TileParameters& Parameters);
	FIntPoint CalculateStartingCoordinates(
		const FIntPoint& Coordinates,
		const FIntPoint& Dimension,
		EInv_TileQuadrant Quadrant) const;
	FInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);
	bool CursorExitedCanvas(
		const FVector2D& BoundaryPosition,
		const FVector2D& BoundarySize,
		const FVector2D& Location);
	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void UnhighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, EInv_GridSlotState GridSlotState);

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

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_HoverItem> HoverItemClass;
	
	UPROPERTY()
	TObjectPtr<UInv_HoverItem> HoverItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Rows{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Columns{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float TileSize{1};

	UPROPERTY()
	FInv_TileParameters TileParameters;
	UPROPERTY()
	FInv_TileParameters LastTileParameters;
	
	int32 ItemDropIndex{INDEX_NONE};
	FInv_SpaceQueryResult CurrentQueryResult;
	bool bMouseWithinCanvas;
	bool bLastMouseWithinCanvas;
	int32 LastHighlightedIndex{INDEX_NONE};
	FIntPoint LastHighlightedDimensions;
};
