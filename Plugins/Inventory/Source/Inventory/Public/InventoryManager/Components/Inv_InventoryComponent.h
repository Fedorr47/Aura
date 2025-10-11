// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManager/FastArray/Inv_FastArray.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChangedSignature, UInv_InventoryItem*, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInventorySignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChangedSignature, const FInv_SlotAvailabilityResult&, AvailabilityResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnInvAttributeChangedSignature, int32, AttributeCode, float, Value, bool, IsIncreasing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipSignature, UInv_InventoryItem*, InventoryItem);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInv_ItemComponent* ItemComponent);

	UFUNCTION(Server, Reliable)
	void Server_AddNewItem(UInv_ItemComponent* ItemsComponent, int32 StackCount);
	UFUNCTION(Server, Reliable)
	void Server_AddStackToItem(
		UInv_ItemComponent* ItemComponent,
		int32 StackCount,
		int32 Remainder);
	UFUNCTION(Server, Reliable)
	void Server_DropItem(UInv_InventoryItem* InventoryItem, int32 StackCount);
	UFUNCTION(Server, Reliable)
	void Server_ConsumeItem(UInv_InventoryItem* InventoryItem);
	UFUNCTION(Server, Reliable)
	void Server_EquipSlotClicked(UInv_InventoryItem* InventoryItemToEquip, UInv_InventoryItem* EquippedSlotToUnequip);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_EquipSlotClicked(UInv_InventoryItem* InventoryItemToEquip, UInv_InventoryItem* InventoryItemToUnequip);

	void ToggleInventoryMenu();
	void AddRepSubObject(UObject* SubObj);
	void SpawnDroppedItem(UInv_InventoryItem* InventoryItem, int32 StackCount);
	UInv_InventoryBase* GetInventoryWidget() const { return InventoryMenuWidget; }

	FInventoryItemChangedSignature OnItemAddedDelegate;
	FInventoryItemChangedSignature OnItemRemovedDelegate;
	FNoRoomInventorySignature OnNoRoomInventoryDelegate;
	FStackChangedSignature OnStackChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnInvAttributeChangedSignature OnInvAttributeChangedDelegate;
	FItemEquipSignature OnItemEquipDelegate;
	FItemEquipSignature OnItemUnequipDelegate;

protected:
	virtual void BeginPlay() override;
private:
	void ConstructInventory();
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	UUserWidget* GeDefaultCursorWidget();

	//----------------------------------------------------------------------------------------//
	TWeakObjectPtr<APlayerController> OwningController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenuWidget;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuWidgetClass;

	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> DefaultCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DefaultCursorWidget;

	bool bInventoryMenuOpen{ false };

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngMin{-85.0f};
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnAngMax{85.0f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMin{10.0f};
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float DropSpawnDistanceMax{50.0f};

	UPROPERTY(EditAnywhere, Category = "Inventory")
	float RelativeSpawnElevation{70.0f};
};
