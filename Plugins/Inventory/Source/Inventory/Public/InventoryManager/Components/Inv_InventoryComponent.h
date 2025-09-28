// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_InventoryComponent.generated.h"

class UInv_InventoryBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChangedSignature, UInv_InventoryItem*, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInventorySignature);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_InventoryComponent();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Inventory")
	void TryAddItem(UInv_DisplayItemsComponent* ItemComponent);

	void ToggleInventoryMenu();

	FInventoryItemChangedSignature OnItemAddedDelegate;
	FInventoryItemChangedSignature OnItemRemovedDelegate;
	FNoRoomInventorySignature OnNoRoomInventoryDelegate;

protected:
	virtual void BeginPlay() override;
private:
	void ConstructInventory();
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	//----------------------------------------------------------------------------------------//
	TWeakObjectPtr<APlayerController> OwningController;

	UPROPERTY()
	TObjectPtr<UInv_InventoryBase> InventoryMenuObj;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;


	bool bInventoryMenuOpen{ false };
};
