// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Inv_Leaf.h"
#include "Inv_Leaf_LabledValue.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_Leaf_LabledValue : public UInv_Leaf
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct() override;
	void SetLabelText(const FText& Text, bool bCollapse);
	void SetValueText(const FText& Text, bool bCollapse);
	//----------------------------------------------------------------------------------------------------------------//
private:
	//----------------------------------------------------------------------------------------------------------------//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueText;

	UPROPERTY(EditAnywhere, Category = Inventory)
	int32 LabelFontSize{12};

	UPROPERTY(EditAnywhere, Category = Inventory)
	int32 ValueFontSize{18};
};
