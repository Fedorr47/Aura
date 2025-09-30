// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "CrossCompilerCommon.h"
#include "IDetailTreeNode.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SlottedItem.generated.h"

class UInv_InventoryItem;
class UImage;

UCLASS()
class INVENTORY_API UInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	UImage* GetImage() const { return ImageIcon; }
	void SetImage(UImage* Image) { ImageIcon = Image; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridIndex(int32 InGridIndex) { GridIndex = InGridIndex; }
	FIntPoint GetGridDimension() const { return GridDimension; }
	void SetGridDimension(FIntPoint InGridDimension) { GridDimension = InGridDimension; }
	UInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	void SetInventoryItem(UInv_InventoryItem* Item);
	void SetImageBrush(const FSlateBrush& Brush) const;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageIcon;

	int32 GridIndex{0};
	FIntPoint GridDimension;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable{false};
};
