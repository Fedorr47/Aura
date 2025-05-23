// Copyright - none


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();

	OnSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnSpellPointsChangedDelegate.AddUObject(this, &USpellMenuWidgetController::OnSpellPointsChanged);
	
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this]
		(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInformation)
		{
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(AbilityTag);
			AbilityInfo.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	});
}

void USpellMenuWidgetController::OnSpellPointsChanged(int32 NewAmount)
{
	OnSpellPointsChangedDelegate.Broadcast(NewAmount);
}
