#include "Items/Fragments/Inv_ItemFragment.h"

#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

bool FInv_InventoryItemFragment::MatchesWidgetTag(const UInv_CompositeBase* CompositeBase) const
{
	return CompositeBase->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

bool FInv_InventoryItemFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	if (!MatchesWidgetTag(CompositeBase))
	{
		return false;
	}
	CompositeBase->Expand();
	return true;
}

bool FInv_ImageFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	if (!FInv_InventoryItemFragment::Assimilate(CompositeBase))
	{
		return false;
	}

	UInv_Leaf_Image* LeafImage = Cast<UInv_Leaf_Image>(CompositeBase);
	if (!IsValid(LeafImage))
	{
		return false;
	}
	
	LeafImage->SetImage(Icon);
	LeafImage->SetBoxSize(IconDimension);
	LeafImage->SetImageSize(IconDimension);

	return true;
}

bool FInv_TextFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	if (!FInv_InventoryItemFragment::Assimilate(CompositeBase))
	{
		return false;
	}

	UInv_Leaf_Text* LeafText = Cast<UInv_Leaf_Text>(CompositeBase);
	if (!IsValid(LeafText))
	{
		return false;
	}

	LeafText->SetText(FragmentText);
	return true;
}

bool FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	if (!FInv_InventoryItemFragment::Assimilate(CompositeBase))
	{
		return false;
	}

	UInv_Leaf_LabledValue* LeafLabledValue = Cast<UInv_Leaf_LabledValue>(CompositeBase);
	if (!IsValid(LeafLabledValue))
	{
		return false;
	}

	LeafLabledValue->SetLabelText(LabelText, bCollapseLabel);

	FNumberFormattingOptions Options;
	Options.MinimumFractionalDigits = MinFractionalDigits;
	Options.MaximumFractionalDigits = MaxFractionalDigits;
	LeafLabledValue->SetValueText(FText::AsNumber(Value, &Options), bCollapseValue);
	
	return true;
}

void FInv_LabeledNumberFragment::Manifest()
{
	Super::Manifest();

	if (bRandomizeOnManifest)
	{
		Value = FMath::FRandRange(MinVal, MaxVal);
	}
	bRandomizeOnManifest = false;
}

void FInv_ConsumableFragment::OnConsume(const UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnHealthEffectDelegate.Broadcast(0);
}

void FInv_HealthPotionFragment::OnConsume(
	const UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnHealthEffectDelegate.Broadcast(HealAmount);
}
