#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Inv_ItemFragments.generated.h"

USTRUCT(BlueprintType)
struct FInv_ItemFragments
{
	GENERATED_BODY()

	FInv_ItemFragments(){}
	FInv_ItemFragments(const FInv_ItemFragments& other) = default;
	FInv_ItemFragments(FInv_ItemFragments&& other) = default;
	FInv_ItemFragments& operator=(const FInv_ItemFragments& other) = default;
	FInv_ItemFragments& operator=(FInv_ItemFragments&& other) = default;
	virtual ~FInv_ItemFragments() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag InTag) { FragmentTag = InTag; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragments
{
	GENERATED_BODY()

	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(FIntPoint InGridSize) { GridSize = InGridSize; }
	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float InGridPadding) { GridPadding = InGridPadding; }
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FIntPoint GridSize{1,1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float GridPadding{0.0f};
};