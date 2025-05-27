// Copyright - none


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHeathChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHeathChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnExperiencePointsChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExperiencePointsChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnHeathChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnMaxHeathChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnManaChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetAuraAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnMaxManaChanged.Broadcast(Data.NewValue);});
	
	if (!IsValid(GetAuraASC()))
	{
		return;
	}

	GetAuraASC()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
	if (GetAuraASC()->bStartupAbilitiesGiven)
	{
		BroadcastAbilityInfo();
	}
	else
	{
		GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
	}
	
	GetAuraASC()->EffectAssetTags.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		for (const FGameplayTag& Tag : AssetTags)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
				const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		}
	}
	);
}

void UOverlayWidgetController::OnExperiencePointsChanged(int32 ExperiencePoints)
{
	ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, L"Level up info is null, please set it in AuraPlayerState Blueprint");

	const int32 Level = LevelUpInfo->FindLevelUpInfoByXP(ExperiencePoints);

	if (const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num(); Level > 0 && Level <= MaxLevel)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = Level == 1 ? 0 : LevelUpInfo->LevelUpInformation[Level-2].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 ExperiencePointsForLevel = ExperiencePoints - PrevLevelUpRequirement;

		const float ExperiencePointsPercent = static_cast<float>(ExperiencePointsForLevel) / static_cast<float>(DeltaLevelRequirement);
		OnExperiencePointsChangedDelegate.Broadcast(ExperiencePointsPercent);
	}
}


void UOverlayWidgetController::OnLevelChanged(int32 LevelCount) const
{
	OnLevelPointsChangedDelegate.Broadcast(LevelCount);
}

void UOverlayWidgetController::OnAbilityEquipped(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag,
	const FGameplayTag& PreviousSlotTag) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlotTag;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInformation->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = StatusTag;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);
}
