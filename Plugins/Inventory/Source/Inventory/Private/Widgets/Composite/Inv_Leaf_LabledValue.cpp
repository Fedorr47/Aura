// Copyright - none


#include "Widgets/Composite/Inv_Leaf_LabledValue.h"

#include "Components/TextBlock.h"

void UInv_Leaf_LabledValue::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo FontInfoLabel = LabelText->GetFont();
	FontInfoLabel.Size = LabelFontSize;
	LabelText->SetFont(FontInfoLabel);
	
	FSlateFontInfo FontInfoValue = ValueText->GetFont();
	FontInfoValue.Size = LabelFontSize;
	ValueText->SetFont(FontInfoValue);
}

void UInv_Leaf_LabledValue::SetLabelText(const FText& Text, bool bCollapse)
{
	if (bCollapse)
	{
		LabelText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	LabelText->SetText(Text);
}

void UInv_Leaf_LabledValue::SetValueText(const FText& Text, bool bCollapse)
{
	if (bCollapse)
	{
		ValueText->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	ValueText->SetText(Text);
}
