#pragma once

#include "Inv_GridTypes.generated.h"

class UInv_InventoryItem;

UENUM(BlueprintType)
enum class EInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

USTRUCT()
struct FInv_SlotAvailability
{
	GENERATED_BODY()

	FInv_SlotAvailability(){}
	FInv_SlotAvailability(
		int32 ItemIndex,
		int32 Room,
		bool bHasItem) :
	Index(ItemIndex),
	AmountToFill(Room),
	bItemAtIndex(bHasItem)
	{}

	int32 Index{INDEX_NONE};
	int32 AmountToFill{0};
	bool bItemAtIndex{false};
};

USTRUCT()
struct FInv_SlotAvailabilityResult
{
	GENERATED_BODY()
public:
	FInv_SlotAvailabilityResult(){}

	TWeakObjectPtr<UInv_InventoryItem> Item;
	int32 TotalRoomToFill{0};
	int32 Remainder{0};
	bool bStackable{false};
	TArray<FInv_SlotAvailability> SlotAvailabilities;
};

UENUM(BlueprintType)
enum class EInv_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FIntPoint TileCoordinates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	EInv_TileQuadrant TileQuadrant{EInv_TileQuadrant::None};
};

inline bool operator==(const FInv_TileParameters& lhs, const FInv_TileParameters& rhs)
{
	return lhs.TileCoordinates == rhs.TileCoordinates
		&& lhs.TileIndex == rhs.TileIndex
		&& lhs.TileQuadrant == rhs.TileQuadrant;
}

USTRUCT(BlueprintType)
struct FInv_SpaceQueryResult
{
	GENERATED_BODY()

	bool bHasSpace{false};

	TWeakObjectPtr<UInv_InventoryItem> ValidItem{nullptr};

	int32 UpperLeftIndex{INDEX_NONE};
};