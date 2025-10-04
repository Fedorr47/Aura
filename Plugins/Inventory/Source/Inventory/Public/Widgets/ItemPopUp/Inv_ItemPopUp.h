// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ItemPopUp.generated.h"


class UButton;
class USlider;
class UTextBlock;
class USizeBox;

DECLARE_DYNAMIC_DELEGATE_TwoParams(FPopUpMenuSplitSignature, int32, SplitAmount, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuDropSignature, int32, Index);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPopUpMenuConsumeSignature, int32, Index);

UCLASS()
class INVENTORY_API UInv_ItemPopUp : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	int32 GetSplitAmount() const;
	void CollapseSplitButton() const;
	void CollapseConsumeButton() const;
	void SetSliderParams(const float Max, const float Value) const;
	FVector2D GetBoxSize() const;
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	
	//----------------------------------------------------------------------------------------------------------------//
	
	FPopUpMenuSplitSignature OnPopUpMenuSplitDelegate;
	FPopUpMenuDropSignature OnPopUpMenuDropDelegate;
	FPopUpMenuConsumeSignature OnPopUpMenuConsumeDelegate;
	
private:

	UFUNCTION()
	void SplitButtonClicked();
	UFUNCTION()
	void DropButtonClicked();
	UFUNCTION()
	void ConsumeButtonClicked();

	UFUNCTION()
	void SliderValueChanged(float Value);
	//----------------------------------------------------------------------------------------------------------------//

	int32 GridIndex{INDEX_NONE};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonSplit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxSplit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonDrop;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxDrop;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonConsume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxConsume;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> SliderSplit;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextSplitAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxRoot;
};
