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

	void ToggleInventoryMenu();
	void AddRepSubObject(UObject* SubObj);

	FInventoryItemChangedSignature OnItemAddedDelegate;
	FInventoryItemChangedSignature OnItemRemovedDelegate;
	FNoRoomInventorySignature OnNoRoomInventoryDelegate;
	FStackChangedSignature OnStackChangedDelegate;

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
	TObjectPtr<UInv_InventoryBase> InventoryMenuObj;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInv_InventoryBase> InventoryMenuClass;

	UPROPERTY(Replicated)
	FInv_InventoryFastArray InventoryList;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UUserWidget> DefaultCursorWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> DefaultCursorWidget;

	bool bInventoryMenuOpen{ false };
};
