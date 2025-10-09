// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/Inv_GridTypes.h"
#include "Widgets/Utils/Inv_WidgetUtils.h"
#include "Inv_InventoryStatics.generated.h"

class UInv_HoverItem;
class UInv_InventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_InventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);
	static bool HasValidInventoryBaseWidget(const APlayerController* PlayerController,
	                                        UInv_InventoryBase*& InventoryBaseWidget);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static EInv_ItemCategory GetItemCategoryFromItemComponent(UInv_ItemComponent* ItemComponent)
	{
		if (!IsValid(ItemComponent))
		{
			return EInv_ItemCategory::None;
		}
		return ItemComponent->GetItemManifest().GetItemCategory();
	}

	template <typename T, typename Func>
	static void ForEach2D(
		TArray<T>& Array,
		int32 Index,
		const FIntPoint& Range2D,
		int32 GridColumns,
		const Func& Function);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static void ItemHovered(APlayerController* PlayerController, UInv_InventoryItem* InventoryItem);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static void ItemUnhovered(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	static UInv_HoverItem* GetHoveredItem(APlayerController* PlayerController);
};

template <typename T, typename Func>
void UInv_InventoryStatics::ForEach2D(
	TArray<T>& Array,
	int32 Index,
	const FIntPoint& Range2D,
	int32 GridColumns,
	const Func& Function)
{
	for (int32 i = 0; i < Range2D.Y; ++i)
	{
		for (int32 j = 0; j < Range2D.X; ++j)
		{
			const FIntPoint Index2D = UInv_WidgetUtils::GetPositionFromIndex(Index, GridColumns) + FIntPoint(j, i);
			const int32 TileIndex = UInv_WidgetUtils::GetIndexFromPosition(Index2D, GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}
