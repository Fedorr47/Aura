// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/Inv_InventoryItem.h"
#include "Types/Inv_GridTypes.h"
#include "Inv_InventoryGrid.generated.h"


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

	UFUNCTION()
	void AddItem(UInv_InventoryItem* InItem);

private:

	void ConstructGrid();

	bool MatchCategory(const UInv_InventoryItem* InItem) const;

	//------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	EInv_ItemCategory ItemCategory;

	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;

	UPROPERTY()
	TArray<TObjectPtr<UInv_GridSlot>> GridSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_GridSlot> GridSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Rows{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Columns{1};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	float TileSize{5};
};
