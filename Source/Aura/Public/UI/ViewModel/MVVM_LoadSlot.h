// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButtonSignature, bool, bEanble);

UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature OnSetWidgetSwitcherIndexDelegate;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButtonSignature OnEnableSelectSlotButtonDelegate;

	UPROPERTY()
	int32 LoadSlotIndex{0};

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus{ESaveSlotStatus::Vacant};

	//----------------------------------------------------------------------------------------------------------------//
	void InitializeSlot();

	// Setters
	void SetPlayerName(FString InPlayerName);
	void SetLoadSlotName(FString InLoadSlotName);
	void SetMapName(FString InMapName);	

	// Getters
	FString GetPlayerName() const { return PlayerName; };
	FString GetLoadSlotName() const { return LoadSlotName; };
	FString GetMapName() const { return MapName; };

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString PlayerName{FString()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString MapName{FString()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess=true))
	FString LoadSlotName{FString()};
};
