// Copyright - none


#include "EquipmentManagment/Components/Inv_EquipmentComponent.h"

#include "EquipmentManagment/EquipActor/Inv_EquipActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InventoryManager/Utils/Inv_InventoryStatics.h"
#include "Items/Inv_InventoryItem.h"
#include "Items/Fragments/Inv_ItemFragment.h"

void UInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerController();
}

void UInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	if (!InventoryComponent.IsValid())
	{
		return;
	}

	if (!InventoryComponent->OnItemEquipDelegate.IsAlreadyBound(this, &ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipDelegate.AddDynamic(this, &ThisClass::OnItemEquipped);
	}
	if (!InventoryComponent->OnItemUnequipDelegate.IsAlreadyBound(this, &ThisClass::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipDelegate.AddDynamic(this, &ThisClass::OnItemUnequipped);
	}
}

void UInv_EquipmentComponent::InitPlayerController()
{
	if (OwningPlayerController = Cast<APlayerController>(GetOwner()); OwningPlayerController.IsValid())
	{
		// TODO: Refactor it
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
		{
			OnPossessedPawnChanged(nullptr, OwnerCharacter);
		}
		else
		{
			OwningPlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UInv_EquipmentComponent::InitializeOwner(APlayerController* InPlayerController)
{
	if (!IsValid(InPlayerController))
	{
		return;
	}
	OwningPlayerController = InPlayerController;
	InitPlayerController();
}

void UInv_EquipmentComponent::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn()); IsValid(OwnerCharacter))
	{
		OwningSkeletalMesh = OwnerCharacter->GetMesh();
	}
	InitInventoryComponent();
}

AInv_EquipActor* UInv_EquipmentComponent::SpawnEquippedActor(
	FInv_EquipmentFragment* EquipmentFragment,
	const FInv_ItemManifest& Manifest,
	USkeletalMeshComponent* AttachMesh)
{
	AInv_EquipActor* SpawnedEquippmentActor = EquipmentFragment->SpawnAttachedActor(AttachMesh);
	SpawnedEquippmentActor->SetEquipmentType(EquipmentFragment->GetEquipmentType());
	SpawnedEquippmentActor->SetOwner(GetOwner());
	EquipmentFragment->SetEquippedActor(SpawnedEquippmentActor);
	return SpawnedEquippmentActor;
}

AInv_EquipActor* UInv_EquipmentComponent::FindEquippedActorByTag(const FGameplayTag& EquipmentType)
{
	auto FoundActor = EquippedActors.FindByPredicate([EquipmentType](const AInv_EquipActor* EquippedActor)
	{
		return EquippedActor->GetEquipmentType().MatchesTagExact(EquipmentType);
	});
	return FoundActor ? *FoundActor : nullptr;
}

void UInv_EquipmentComponent::RemoveEquippedActor(const FGameplayTag& EquipmentType)
{
	if (AInv_EquipActor* EquippedActor = FindEquippedActorByTag(EquipmentType); IsValid(EquippedActor))
	{
		EquippedActors.Remove(EquippedActor);
		EquippedActor->Destroy();
	}
}

void UInv_EquipmentComponent::SetOwningSkeletalMesh(USkeletalMeshComponent* InOwningMesh)
{
	OwningSkeletalMesh = InOwningMesh;
}

void UInv_EquipmentComponent::OnItemEquipped(UInv_InventoryItem* EquippedItem)
{
	if (!IsValid(EquippedItem))
	{
		return;
	}
	if (!OwningPlayerController->HasAuthority())
	{
		return;
	}

	FInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment)
	{
		return;
	}

	if (!bIsProxy)
	{
		EquipmentFragment->OnEquip(UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get()));
	}

	if (!OwningSkeletalMesh.IsValid())
	{
		return;
	}
	AInv_EquipActor* SpawnedEquipActor =
		SpawnEquippedActor(EquipmentFragment, ItemManifest, OwningSkeletalMesh.Get());

	EquippedActors.Add(SpawnedEquipActor);
}

void UInv_EquipmentComponent::OnItemUnequipped(UInv_InventoryItem* UnequippedItem)
{
	if (!IsValid(UnequippedItem))
	{
		return;
	}
	if (!OwningPlayerController->HasAuthority())
	{
		return;
	}

	FInv_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FInv_EquipmentFragment>();
	if (!EquipmentFragment)
	{
		return;
	}

	if (!bIsProxy)
	{
		EquipmentFragment->OnUnequip(UInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get()));
	}

	RemoveEquippedActor(EquipmentFragment->GetEquipmentType());
}

