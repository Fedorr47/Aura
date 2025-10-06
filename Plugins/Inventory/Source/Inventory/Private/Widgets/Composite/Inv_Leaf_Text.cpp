// Copyright - none


#include "Widgets/Composite/Inv_Leaf_Text.h"

#include "Components/TextBlock.h"

void UInv_Leaf_Text::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo FontInfo = LeafText->GetFont();
	FontInfo.Size = FontSize;
	LeafText->SetFont(FontInfo);
}

void UInv_Leaf_Text::SetText(const FText& Text)
{
	LeafText->SetText(Text);
}
