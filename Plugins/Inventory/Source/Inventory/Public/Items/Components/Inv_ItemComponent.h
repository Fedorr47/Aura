// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	FString GetPickupMessage() {return PickupMessage;};
	void InitItemManifest(FInv_ItemManifest CopyOfManifest);
	FInv_ItemManifest GetItemManifest() const { return ItemManifest; }
	FInv_ItemManifest& GetItemManifestMutable() { return ItemManifest; }
	void PickedUp();
	
protected:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void OnPickedUp();

private:

	UPROPERTY(EditAnywhere, Category = Inventory, Replicated)
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Pickup Widget")
	FString PickupMessage;
};
