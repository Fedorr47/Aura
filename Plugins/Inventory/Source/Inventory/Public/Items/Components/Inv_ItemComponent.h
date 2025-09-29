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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	FString GetPickupMessage() {return PickupMessage;};

	FInv_ItemManifest GetItemManifest() const { return ItemManifest; }

	
protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = Inventory, Replicated)
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditAnywhere, Category = "Pickup Widget")
	FString PickupMessage;
};
