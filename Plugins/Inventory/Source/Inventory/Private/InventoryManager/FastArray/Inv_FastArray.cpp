#include "InventoryManager/FastArray/Inv_FastArray.h"

#include "ToolMenusEditor.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Tests/ToolMenusTestUtilities.h"

TArray<UInv_InventoryItem*> FInv_InventoryFastArray::GetAllItems() const
{
	TArray<UInv_InventoryItem*> ReturnValue;
	ReturnValue.Reserve(Entries.Num());
	for (const FInv_InventoryEntry& Entry : Entries)
	{
		if (!IsValid(Entry.Item))
		{
			continue;
		}
		ReturnValue.Add(Entry.Item);
	}
	return ReturnValue;
}

void FInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemoveIndices, int32 FinalSize)
{
	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	for (int32 Index : RemoveIndices)
	{
		InventoryComponent->OnItemRemovedDelegate.Broadcast(Entries[Index].Item);
	}
}

void FInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	for (int32 Index : AddedIndices)
	{
		InventoryComponent->OnItemAddedDelegate.Broadcast(Entries[Index].Item);
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_ItemComponent* InItemComponent)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	UInv_InventoryComponent* InventoryComponent = Cast<UInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(InventoryComponent))
	{
		return nullptr;
	}

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = InItemComponent->GetItemManifest().Manifest(OwningActor);

	InventoryComponent->AddRepSubObject(NewEntry.Item);
	MarkItemDirty(NewEntry);

	return NewEntry.Item;
}

UInv_InventoryItem* FInv_InventoryFastArray::AddEntry(UInv_InventoryItem* InItem)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FInv_InventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Item = InItem;
	
	MarkItemDirty(NewEntry);
	
	return InItem;
}

void FInv_InventoryFastArray::RemoveEntry(UInv_InventoryItem* InItem)
{
	for (auto EntryIterator = Entries.CreateIterator(); EntryIterator; ++EntryIterator)
	{
		FInv_InventoryEntry& Entry = *EntryIterator;
		if (Entry.Item == InItem)
		{
			EntryIterator.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

UInv_InventoryItem* FInv_InventoryFastArray::FindFirstItemByType(const FGameplayTag& ItemType)
{
	FInv_InventoryEntry* FoundItem = Entries.FindByPredicate([ItemType](const FInv_InventoryEntry& Entry)
	{
		return IsValid(Entry.Item) && Entry.Item->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
	});
	return FoundItem ? FoundItem->Item : nullptr;
}
