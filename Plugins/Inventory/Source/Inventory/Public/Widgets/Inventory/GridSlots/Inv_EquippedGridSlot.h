// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inv_GridSlot.h"
#include "Inv_EquippedGridSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClickedSignature, UInv_EquippedGridSlot*, GridSloot, const FGameplayTag&, EquipmentTypeTag);

UCLASS()
class INVENTORY_API UInv_EquippedGridSlot : public UInv_GridSlot
{
	GENERATED_BODY()
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//----------------------------------------------------------------------------------------------------------------//
	
	FEquippedGridSlotClickedSignature OnFEquippedGridSlotClickedDelegate;
	
private:

	//----------------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, Category = Inventory, meta = (Categories = "Inventory.GameItems.Equipment"))
	FGameplayTag EquipmentTypeTag;
};
