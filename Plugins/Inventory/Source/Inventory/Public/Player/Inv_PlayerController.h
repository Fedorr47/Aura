// Copyright - none

#pragma once

#include "CoreMinimal.h"

#include "Inv_PlayerController.generated.h"

class UInv_HUDWidget;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class INVENTORY_API AInv_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AInv_PlayerController();
protected:
	void BeginPlay() override;
	void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetInventoryHUDWidget(UInv_HUDWidget* InventoryHUDWidget);

private:

	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem();
	void TraceCursorForItem();

	//---------------------------------------------------------------------------------------------------------------------//	
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	double TraceLength{1000.0f};

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
};
