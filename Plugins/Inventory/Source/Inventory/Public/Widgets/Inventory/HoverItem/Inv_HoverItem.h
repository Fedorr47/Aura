// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Inv_HoverItem.generated.h"

class UTextBlock;
class UInv_InventoryItem;
class UImage;

UCLASS()
class INVENTORY_API UInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetImageBrush(const FSlateBrush& Brush) const;
	void UpdateStackCount(const int32 NewStackCount) const;
	FGameplayTag GetItemType() const;
	int32 GetStackCount() const { return StackCount; }
	bool IsStackable() const {return bIsStackable;}
	void SetIsStackable(bool bIsStacks);
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }
	void SetPreviousGridIndex(int32 InPreviousGridIndex) {PreviousGridIndex = InPreviousGridIndex; }
	FIntPoint GetGridDimension() const { return GridDimension; }
	void SetGridDimension(const FIntPoint& InGridDimension) {GridDimension = InGridDimension; }
	UInv_InventoryItem* GetInventoryItem() const;
	void SetInventoryItem(UInv_InventoryItem* InInventoryItem);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextStackCount;

	int32 PreviousGridIndex{0};
	FIntPoint GridDimension;
	TWeakObjectPtr<UInv_InventoryItem> InventoryItem;
	bool bIsStackable{false};
	int32 StackCount{0};
};
