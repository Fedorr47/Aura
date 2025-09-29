#pragma once

#include "CoreMinimal.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "StructUtils/InstancedStruct.h"
#include "Types/Inv_GridTypes.h"

#include "Inv_ItemManifest.generated.h"

/**
 * The Item Manifest contains all of the necessary
 * data for creating a new Inventory data
 */

USTRUCT(BlueprintType)
struct INVENTORY_API FInv_ItemManifest
{
	GENERATED_BODY()
public:

	UInv_InventoryItem* Manifest(UObject* NewOuter);
	EInv_ItemCategory GetItemCategory() const { return ItemCategory; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	EInv_ItemCategory ItemCategory{EInv_ItemCategory::MAX};
};