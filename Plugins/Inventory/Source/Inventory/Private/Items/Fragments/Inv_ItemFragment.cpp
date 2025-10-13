#include "Items/Fragments/Inv_ItemFragment.h"

#include "EquipmentManagment/EquipActor/Inv_EquipActor.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/Composite/Inv_CompositeBase.h"
#include "Widgets/Composite/Inv_Leaf_Image.h"
#include "Widgets/Composite/Inv_Leaf_LabledValue.h"
#include "Widgets/Composite/Inv_Leaf_Text.h"

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
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

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
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

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
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

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
bool FInv_LabeledNumberFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	FInv_InventoryItemFragment::Assimilate(CompositeBase);
	if (!MatchesWidgetTag(CompositeBase))
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

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
void FInv_ConsumableFragment::OnConsume(UInv_InventoryComponent* InventoryComponent)
{
	for (TInstancedStruct<FInv_ConsumeModifier>& ConsumeModifier : ConsumeModifiers)
	{
		FInv_ConsumeModifier& ModifierRef = ConsumeModifier.GetMutable();
		ModifierRef.OnConsume(InventoryComponent);
	}
}

bool FInv_ConsumableFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	Super::Assimilate(CompositeBase);
	
	for (const TInstancedStruct<FInv_ConsumeModifier>& ConsumeModifier : ConsumeModifiers)
	{
		const FInv_ConsumeModifier& ModifierRef = ConsumeModifier.Get();
		if (!ModifierRef.Assimilate(CompositeBase))
		{
			return false;
		}
	}

	return true;
}

void FInv_ConsumableFragment::Manifest()
{
	for (TInstancedStruct<FInv_ConsumeModifier>& ConsumeModifier : ConsumeModifiers)
	{
		FInv_ConsumeModifier& ModifierRef = ConsumeModifier.GetMutable();
		ModifierRef.Manifest();
	}
}

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
void FInv_HealthPotionFragment::OnConsume(
	UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnInvAttributeChangedDelegate.Broadcast(
		static_cast<int32>(EAttributeCodes::Health),GetValue(), true);
}

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
void FInv_StrengthModifier::OnEquip(UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnInvAttributeChangedDelegate.Broadcast(
		static_cast<int32>(EAttributeCodes::Strength),GetValue(), true);
}

void FInv_StrengthModifier::OnUnequip(UInv_InventoryComponent* InventoryComponent)
{
	InventoryComponent->OnInvAttributeChangedDelegate.Broadcast(
		static_cast<int32>(EAttributeCodes::Strength), GetValue(), false);
}

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/

void FInv_EquipmentFragment::OnEquip(UInv_InventoryComponent* InventoryComponent)
{
	if (bEquipped)
	{
		return;
	}
	bEquipped = true;
	for (TInstancedStruct<FInv_EquipModifier>& EquipModifier : EquipModifiers)
	{
		FInv_EquipModifier& ModifierRef = EquipModifier.GetMutable();
		ModifierRef.OnEquip(InventoryComponent);
	}
}

void FInv_EquipmentFragment::OnUnequip(UInv_InventoryComponent* InventoryComponent)
{
	if (!bEquipped)
	{
		return;
	}
	bEquipped = false;
	for (TInstancedStruct<FInv_EquipModifier>& EquipModifier : EquipModifiers)
	{
		FInv_EquipModifier& ModifierRef = EquipModifier.GetMutable();
		ModifierRef.OnUnequip(InventoryComponent);
	}
}

bool FInv_EquipmentFragment::Assimilate(UInv_CompositeBase* CompositeBase) const
{
	Super::Assimilate(CompositeBase);
	
	for (const TInstancedStruct<FInv_EquipModifier>& EquipModifier : EquipModifiers)
	{
		const FInv_EquipModifier& ModifierRef = EquipModifier.Get();
		if (!ModifierRef.Assimilate(CompositeBase))
		{
			return false;
		}
	}

	return true;
}

void FInv_EquipmentFragment::Manifest()
{
	for (TInstancedStruct<FInv_EquipModifier>& EquipModifier : EquipModifiers)
	{
		FInv_EquipModifier& ModifierRef = EquipModifier.GetMutable();
		ModifierRef.Manifest();
	}
}

AInv_EquipActor* FInv_EquipmentFragment::SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const
{
	if (!IsValid(EquipActorClass) || !IsValid(AttachMesh))
	{
		return nullptr;
	}
	AInv_EquipActor* SpawnedActor = AttachMesh->GetWorld()->SpawnActor<AInv_EquipActor>(EquipActorClass);
	SpawnedActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketAttachPoint);

	return SpawnedActor;
}

void FInv_EquipmentFragment::DestroyAttachedActor() const
{
	if (EquippedActor.IsValid())
	{
		EquippedActor->Destroy();
	}
}

void FInv_EquipmentFragment::SetEquippedActor(AInv_EquipActor* InEquippedActor)
{
	EquippedActor = InEquippedActor;
}

//--------------------------------------------------------------------------------------------------------------------//
/*																													  */
//--------------------------------------------------------------------------------------------------------------------/
