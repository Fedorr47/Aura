// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
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
	TSubclassOf<UMVVM_LoadScreen::UUMVVM_LoadSlot> LoadSlotViewModelClass;
	
	//----------------------------------------------------------------------------------------------------------------//
	void InitializeLoadSlots();
	
	// Setters
	void SetNumLoadSlots(int32 InNumLoadSlots);

	// Getters
	int32 GetNumLoadSlots() const { return NumLoadSlots; };
	
private:
	UPROPERTY()
	TMap<int32, UMVVM_LoadScreen::UUMVVM_LoadSlot*> LoadSlotView;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"))
	int32 NumLoadSlots;
	
};
