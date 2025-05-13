// Copyright - none


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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

void UOverlayWidgetController::OnInitializedStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
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


