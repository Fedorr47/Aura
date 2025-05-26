// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FSpellGlobeSelectedSignature,
	bool, bSpendButtonEnabled,
	bool, bEquipButtonEnabled,
	FString, DescriptionString,
	FString, NextLevelDescriptionString);

struct FSelectedAbility
{
	FGameplayTag AbilityTag{FGameplayTag()};
	FGameplayTag StatusTag{FGameplayTag()};
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Spell Points")
	FOnStatChangedSignature OnSpellPointsChangedDelegate;

	UFUNCTION(BlueprintCallable, Category = "GAS|Spell Points")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "GAS|Spell Points")
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable, Category = "GAS|Spell Points")
	void SpellGlobeDeselected();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Spell Points")
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;

private:
	void OnSpellPointsChanged(int32 NewAmount);
	static void OnShouldEnableButtons(
		const FGameplayTag& AbilityStatus,
		int32 SpellPoints,
		bool& bShouldEnableSPButton,
		bool& bShouldEnableEquipButton);

	//---------------------------------------------------------//

	FSelectedAbility SelectedAbility {FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked};
	int32 CurrentSpellPoints {0};
};
