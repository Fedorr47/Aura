// Copyright - none


#include "Widgets/ItemPopUp/Inv_ItemPopUp.h"

#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UInv_ItemPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ButtonSplit->OnClicked.AddDynamic(this, &ThisClass::SplitButtonClicked);
	ButtonDrop->OnClicked.AddDynamic(this, &ThisClass::DropButtonClicked);
	ButtonConsume->OnClicked.AddDynamic(this, &ThisClass::ConsumeButtonClicked);
	SliderSplit->OnValueChanged.AddDynamic(this, &ThisClass::SliderValueChanged);
}

FVector2D UInv_ItemPopUp::GetBoxSize() const
{
	return FVector2D(SizeBoxRoot->GetWidthOverride(), SizeBoxRoot->GetHeightOverride());
}

void UInv_ItemPopUp::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	RemoveFromParent();
}

int32 UInv_ItemPopUp::GetSplitAmount() const
{
	return FMath::Floor(SliderSplit->GetValue());
}

void UInv_ItemPopUp::SplitButtonClicked()
{
	if (OnPopUpMenuSplitDelegate.ExecuteIfBound(GetSplitAmount(), GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::DropButtonClicked()
{
	if (OnPopUpMenuDropDelegate.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::ConsumeButtonClicked()
{
	if (OnPopUpMenuConsumeDelegate.ExecuteIfBound(GridIndex))
	{
		RemoveFromParent();
	}
}

void UInv_ItemPopUp::SliderValueChanged(float Value)
{
	TextSplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}

void UInv_ItemPopUp::CollapseSplitButton() const
{
	SizeBoxSplit->SetVisibility(ESlateVisibility::Collapsed);
	SliderSplit->SetVisibility(ESlateVisibility::Collapsed);
	TextSplitAmount->SetVisibility(ESlateVisibility::Collapsed);
}

 void UInv_ItemPopUp::CollapseConsumeButton() const
{
	SizeBoxDrop->SetVisibility(ESlateVisibility::Collapsed);
}

void UInv_ItemPopUp::SetSliderParams(const float Max, const float Value) const
{
	SliderSplit->SetMaxValue(Max);
	SliderSplit->SetValue(1);
	SliderSplit->SetValue(Value);
	TextSplitAmount->SetText(FText::AsNumber(FMath::Floor(Value)));
}
