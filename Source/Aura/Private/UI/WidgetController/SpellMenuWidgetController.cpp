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
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this]
		(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 InLevel)
	{
		if (SelectedAbility.AbilityTag.MatchesTag(AbilityTag))
		{
			SelectedAbility.StatusTag = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			OnShouldEnableButtons(
				StatusTag,
				CurrentSpellPoints,
				bEnableSpendPoints,
				bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag,Description, NextLevelDescription);
			OnSpellGlobeSelectedDelegate.Broadcast(
				bEnableSpendPoints,
				bEnableEquip,
				Description,
				NextLevelDescription
				);
		}
		if (AbilityInformation)
		{
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(AbilityTag);
			AbilityInfo.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	});
	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	GetAuraPS()->OnSpellPointsChangedDelegate.AddUObject(this, &USpellMenuWidgetController::OnSpellPointsChanged);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInformation->FindAbilityInfoByTag(AbilityTag).AbilityType;
		OnStopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
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

	SelectedAbility.AbilityTag = AbilityTag;
	SelectedAbility.StatusTag = AbilityStatus;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	OnShouldEnableButtons(
		AbilityStatus,
		SpellPoints,
		bEnableSpendPoints,
		bEnableEquip);

	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag,Description, NextLevelDescription);
	OnSpellGlobeSelectedDelegate.Broadcast(
		bEnableSpendPoints,
		bEnableEquip,
		Description,
		NextLevelDescription);
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}
	const FGameplayTag& SelectedAbilityType = AbilityInformation->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
	{
		return;
	}

	GetAuraASC()->ServerEquipAbility(SelectedAbility.AbilityTag, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag,
	const FGameplayTag& PreviousSlotTag)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.StatusTag = PreviousSlotTag;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInformation->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);

	OnStopWaitingForEquipDelegate.Broadcast(AbilityInformation->FindAbilityInfoByTag(AbilityTag).AbilityType);
}

void USpellMenuWidgetController::OnSpellPointsChanged(int32 NewAmount)
{
	OnSpellPointsChangedDelegate.Broadcast(NewAmount);
	CurrentSpellPoints = NewAmount;

	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;

	OnShouldEnableButtons(
		SelectedAbility.StatusTag,
		CurrentSpellPoints,
		bEnableSpendPoints,
		bEnableEquip);
	
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(
		SelectedAbility.AbilityTag,Description, NextLevelDescription);
	OnSpellGlobeSelectedDelegate.Broadcast(
		bEnableSpendPoints,
		bEnableEquip,
		Description,
		NextLevelDescription);
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

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoints(SelectedAbility.AbilityTag);
	}
}

void USpellMenuWidgetController::SpellGlobeDeselected()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInformation->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
		OnStopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
	SelectedAbility.AbilityTag = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.StatusTag = FAuraGameplayTags::Get().Abilities_Status_Locked;

	OnSpellGlobeSelectedDelegate.Broadcast(
		false,
		false,
		FString(),
		FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInformation->FindAbilityInfoByTag(SelectedAbility.AbilityTag).AbilityType;
	OnWaitForEquipSelectionDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.AbilityTag);
	if (SelectedStatus.MatchesTagExact( FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.AbilityTag);
	}
}
