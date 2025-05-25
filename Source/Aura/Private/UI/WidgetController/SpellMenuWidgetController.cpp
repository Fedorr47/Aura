// Copyright - none


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
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

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(FAuraGameplayTags::Get().Abilities_None);
	const bool bSpecValid = AbilitySpec != nullptr;
	if (!bTagValid || !bSpecValid || bTagNone)
	{
		AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	OnShouldEnableButtons(
		AbilityStatus,
		SpellPoints,
		bEnableSpendPoints,
		bEnableEquip);
	OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void USpellMenuWidgetController::OnSpellPointsChanged(int32 NewAmount)
{
	OnSpellPointsChangedDelegate.Broadcast(NewAmount);
}

void USpellMenuWidgetController::OnShouldEnableButtons(
	const FGameplayTag& AbilityStatus,
	int32 SpellPoints,
	bool& bShouldEnableSPButton,
	bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags &GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSPButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSPButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSPButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSPButton = true;
		}
	}
}
