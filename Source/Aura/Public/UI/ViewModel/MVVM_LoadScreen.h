// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "MVVM_LoadScreen.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	//----------------------------------------------------------------------------------------------------------------//
	void InitializeLoadSlots();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex, UPARAM(ref) FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);
	
	// Setters
	void SetNumLoadSlots(int32 InNumLoadSlots);

	// Getters
	int32 GetNumLoadSlots() const { return NumLoadSlots; };
	
private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlotView;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 NumLoadSlots;
	
};
