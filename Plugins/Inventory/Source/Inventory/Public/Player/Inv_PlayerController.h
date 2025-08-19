// Copyright - none

#pragma once

#include "CoreMinimal.h"

#include "Inv_PlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
	void SetupInputComponent() override;

private:

	void PrimaryInteract();

//---------------------------------------------------------------------------------------------------------------------//	
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TObjectPtr<UInputAction> PrimaryInteractAction;
};
