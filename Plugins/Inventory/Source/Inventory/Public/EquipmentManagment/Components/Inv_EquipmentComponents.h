// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_EquipmentComponents.generated.h"

struct FGameplayTag;
class AInv_EquipActor;
struct FInv_ItemManifest;
struct FInv_EquipmentFragment;
class UInv_InventoryComponent;
class APlayerController;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_EquipmentComponents : public UActorComponent
{
	GENERATED_BODY()
public:
//--------------------------------------------------------------------------------------------------------------------//
protected:
	virtual void BeginPlay() override;
//--------------------------------------------------------------------------------------------------------------------//	
private:
	UFUNCTION()
	void OnItemEquipped(UInv_InventoryItem* EquippedItem);
	UFUNCTION()
	void OnItemUnequipped(UInv_InventoryItem* UnequippedItem);

	void InitInventoryComponent();
	AInv_EquipActor* SpawnEquippedActor(
		FInv_EquipmentFragment* EquipmentFragment,
		const FInv_ItemManifest& Manifest,
		USkeletalMeshComponent* AttachMesh);
	AInv_EquipActor* FindEquippedActorByTag(const FGameplayTag& EquipmentType);
	void RemoveEquippedActor(const FGameplayTag& EquipmentType);
//--------------------------------------------------------------------------------------------------------------------//
	TWeakObjectPtr<UInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UPROPERTY()
	TArray<TObjectPtr<AInv_EquipActor>> EquippedActors;
};
