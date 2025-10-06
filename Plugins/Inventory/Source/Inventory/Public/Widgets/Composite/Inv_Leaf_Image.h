// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Inv_Leaf.h"
#include "Inv_Leaf_Image.generated.h"


class USizeBox;
class UImage;

UCLASS()
class INVENTORY_API UInv_Leaf_Image : public UInv_Leaf
{
	GENERATED_BODY()
public:
	//virtual void ApplyFunction(FuncType Function) override;

	void SetImage(UTexture2D* Texture);
	void SetBoxSize(const FVector2D& Size);
	void SetImageSize(const FVector2D& Size);
	FVector2D GetImageSize() const;
	//----------------------------------------------------------------------------------------------------------------//
private:
	//----------------------------------------------------------------------------------------------------------------//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxIcon;
};
