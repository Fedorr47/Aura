// Copyright - none


#include "Widgets/Composite/Inv_Leaf_Image.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UInv_Leaf_Image::SetImage(UTexture2D* Texture)
{
	ImageIcon->SetBrushFromTexture(Texture);
}

void UInv_Leaf_Image::SetBoxSize(const FVector2D& Size)
{
	SizeBoxIcon->SetWidthOverride(Size.X);
	SizeBoxIcon->SetWidthOverride(Size.Y);
}

void UInv_Leaf_Image::SetImageSize(const FVector2D& Size)
{
	ImageIcon->SetDesiredSizeOverride(Size);
}

FVector2D UInv_Leaf_Image::GetImageSize() const
{
	return ImageIcon->GetDesiredSize();
}
