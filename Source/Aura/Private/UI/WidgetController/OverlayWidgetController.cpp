// Copyright - none


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHeathChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHeathChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnExperiencePointsChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExperiencePointsChanged);
	AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnLevelChanged);
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnHeathChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnMaxHeathChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnManaChanged.Broadcast(Data.NewValue);});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data){
			OnMaxManaChanged.Broadcast(Data.NewValue);});

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!IsValid(AuraASC))
	{
		return;
	}

	if (AuraASC->bStartupAbilitiesGiven)
	{
		OnInitializedStartupAbilities(AuraASC);
	}
	else
	{
		AuraASC->AbilitiesGivenDelegate.AddUObject(this, &ThisClass::OnInitializedStartupAbilities);
	}
	
	AuraASC->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnExperiencePointsChanged(int32 ExperiencePoints) const
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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

void UOverlayWidgetController::OnInitializedStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent) const
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo AbilityInfo =  AbilityInformation->FindAbilityInfoByTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		AbilityInfo.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(AbilityInfo);
	});
	
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}


