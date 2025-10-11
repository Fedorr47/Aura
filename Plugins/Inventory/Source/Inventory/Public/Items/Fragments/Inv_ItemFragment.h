#pragma once

#include "CoreMinimal.h"
#include "EditorCategoryUtils.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "Widgets/Composite/Inv_CompositeBase.h"

#include "Inv_ItemFragment.generated.h"

class UInv_InventoryComponent;
class APlayerController;
class UInv_CompositeBase;

UENUM()
enum class EAttributeCodes : uint8
{
	Health,
	Strength,

	None
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_ItemFragment
{
	GENERATED_BODY()

	FInv_ItemFragment(){}
	FInv_ItemFragment(const FInv_ItemFragment& other) = default;
	FInv_ItemFragment(FInv_ItemFragment&& other) = default;
	FInv_ItemFragment& operator=(const FInv_ItemFragment& other) = default;
	FInv_ItemFragment& operator=(FInv_ItemFragment&& other) = default;
	virtual ~FInv_ItemFragment() {}

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(FGameplayTag InTag) { FragmentTag = InTag; }
	virtual void Manifest() {}

private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories="Inventory.FragmentsTags"))
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_InventoryItemFragment : public FInv_ItemFragment
{
	GENERATED_BODY()
	
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const;

	bool bRandomizeOnManifest{true};
	
protected:
	bool MatchesWidgetTag(const UInv_CompositeBase* CompositeBase) const;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_GridFragment : public FInv_ItemFragment
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

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_ImageFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetIcon() const { return Icon; }
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TObjectPtr<UTexture2D> Icon{nullptr};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FVector2D IconDimension{44.0f, 44.0f};
};

USTRUCT(BlueprintType)
struct FInv_TextFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()
	FText GetText() const { return FragmentText; }
	void SetText(FText InText) { FragmentText = InText; }
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const override;

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText FragmentText;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_LabeledNumberFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()
	FText GetText() const { return LabelText; }
	void SetText(FText InText) { LabelText = InText; }
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const override;
	virtual void Manifest() override;
	float GetValue() const { return Value; }
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	FText LabelText;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	float Value{0.0f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MinVal{0.0f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float MaxVal{0.0f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseLabel{false};
	
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bCollapseValue{false};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MinFractionalDigits{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxFractionalDigits{1};
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_StackableFragment : public FInv_ItemFragment
{
	GENERATED_BODY()

	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 InStackCount) { StackCount = InStackCount; }

private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 MaxStackSize{1};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 StackCount{1};
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_ConsumableFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(UInv_InventoryComponent* InventoryComponent);
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const override;
	virtual void Manifest() override;
private:

	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FInv_ConsumeModifier>> ConsumeModifiers;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_ConsumeModifier : public FInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnConsume(UInv_InventoryComponent* InventoryComponent){}
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_HealthPotionFragment : public FInv_ConsumeModifier
{
	GENERATED_BODY()
	
	virtual void OnConsume(UInv_InventoryComponent* InventoryComponent) override;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/

USTRUCT(BlueprintType)
struct FInv_EquipModifier : public FInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnEquip(UInv_InventoryComponent* InventoryComponent){}
	virtual void OnUnequip(UInv_InventoryComponent* InventoryComponent){}
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/

USTRUCT(BlueprintType)
struct FInv_StrengthModifier : public FInv_EquipModifier
{
	GENERATED_BODY()

	virtual void OnEquip(UInv_InventoryComponent* InventoryComponent) override;
	virtual void OnUnequip(UInv_InventoryComponent* InventoryComponent) override;
};

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
USTRUCT(BlueprintType)
struct FInv_EquipmentFragment : public FInv_InventoryItemFragment
{
	GENERATED_BODY()

	bool bEquipped{false};
	virtual void OnEquip(UInv_InventoryComponent* InventoryComponent);
	virtual void OnUnequip(UInv_InventoryComponent* InventoryComponent);
	virtual bool Assimilate(UInv_CompositeBase* CompositeBase) const override;
	
private:

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TArray<TInstancedStruct<FInv_EquipModifier>> EquipModifiers;
};