// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_GridSlot.generated.h"

class UInv_InventoryItem;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEventSignatuure, int32, GridIndex, const FPointerEvent&, MouseEvent);

UENUM(BlueprintType)
enum class EInv_GridSlotState : uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

UCLASS()
class INVENTORY_API UInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	int32 GetTileIndex() const { return TileIndex; }
	void SetTileIndex(const int32 Index) { TileIndex = Index; }
	EInv_GridSlotState GetGridSlotState() const { return GridSlotState; }
	void SetInventoryItem(UInv_InventoryItem* Item);
	TWeakObjectPtr<UInv_InventoryItem> GetInventoryItem() const {return InventoryItem; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(const int32 Count) { StackCount = Count; }
	int32 GetUpperLeftIndex() const { return UpperLeftIndex; }
	void SetUpperLeftIndex(const int32 Index) { UpperLeftIndex = Index; }
	bool GetAvailable() const {return bAvailable; }
	void SetAvailable(bool bIsAvailable) { bAvailable = bIsAvailable; }
	
	void SetUnoccupiedTexture();
	void SetOccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	//----------------------------------------------------------------------------------------------------------------//
	FGridSlotEventSignatuure OnGridSlotClickedDelegate;
	FGridSlotEventSignatuure OnGridSlotHoveredDelegate;
	FGridSlotEventSignatuure OnGridSlotUnhoveredDelegate;
	
private:
	
	//----------------------------------------------------------------------------------------------------------------//

	int32 TileIndex{INDEX_NONE};
	int32 StackCount{0};
	int32 UpperLeftIndex{INDEX_NONE};
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bAvailable{true};
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FSlateBrush Brush_GrayedOut;

	EInv_GridSlotState GridSlotState;
};
