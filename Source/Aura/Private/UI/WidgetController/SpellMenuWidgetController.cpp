// Copyright - none


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
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
