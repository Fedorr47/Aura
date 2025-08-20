// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inv_DisplayItemsComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_DisplayItemsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_DisplayItemsComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FString GetPickupMessage() {return PickupMessage;};

private:

	UPROPERTY(EditAnywhere, Category = "Pickup Widget")
	FString PickupMessage;
};
