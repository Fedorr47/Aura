#include "Items/Manifest/Inv_ItemManifest.h"

#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Items/Fragments/Inv_ItemFragment.h"
#include "Widgets/Composite/Inv_CompositeBase.h"

UInv_InventoryItem* FInv_ItemManifest::Manifest(UObject* NewOuter)
{
	UInv_InventoryItem* Item = NewObject<UInv_InventoryItem>(NewOuter, UInv_InventoryItem::StaticClass());
	Item->SetItemManifest(*this);

	for (auto& Fragment : Item->GetItemManifestMutable().GetFragmentsMutable())
	{
		Fragment.GetMutable().Manifest();	
	}
	ClearFragments();
	return Item;
}

void FInv_ItemManifest::AssimilateInventoryFragments(UInv_CompositeBase* CompositeBase) const
{
	const TArray<const FInv_InventoryItemFragment*>& ResFragments = GetAllFragmentsOfType<FInv_InventoryItemFragment>();
	for (const FInv_InventoryItemFragment* Fragment : ResFragments)
	{
		CompositeBase->ApplyFunction([&Fragment](UInv_CompositeBase* CompositeBaseWidget)
		{
			Fragment->Assimilate(CompositeBaseWidget);
		});
	}
}

void FInv_ItemManifest::SpawnPickupActor(
	const UObject* WorldContextObject,
	const FVector& SpawnLocation,
	const FRotator& SpawnRotation)
{
	if (!IsValid(WorldContextObject) || !IsValid(PickupActorClass))
	{
		return;
	}
	AActor* PickupSpawnable = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);
	if (!IsValid(PickupSpawnable))
	{
		return;
	}

	UInv_ItemComponent* ItemComponent = PickupSpawnable->FindComponentByClass<UInv_ItemComponent>();
	check(ItemComponent);

	ItemComponent->InitItemManifest(*this);;
}

void FInv_ItemManifest::ClearFragments()
{
	for (auto& Fragment : GetFragmentsMutable())
	{
		Fragment.Reset();	
	}
	Fragments.Empty();
}
