// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Widgets/Inventory/SlottedItems/Inv_SlottedItem.h"
#include "Inv_EquippedSlottedItem.generated.h"

class UInv_EquippedSlottedItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClickedSignature, UInv_EquippedSlottedItem*, SlottedItem);

UCLASS()
class INVENTORY_API UInv_EquippedSlottedItem : public UInv_SlottedItem
{
	GENERATED_BODY()
public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	void SetEquippedSlottedItemType(FGameplayTag InGameplayTag) { EquippedTypeTag = InGameplayTag; }
	FGameplayTag GetEquippedSlottedItemType() const { return EquippedTypeTag; }
	//----------------------------------------------------------------------------------------------------------------//

	FEquippedSlottedItemClickedSignature OnClickedSlottedItemClickedDelegate;
	
private:

	
	//----------------------------------------------------------------------------------------------------------------//
	UPROPERTY()
	FGameplayTag EquippedTypeTag;
};
