// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Data/AbilityInfo.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*Asset tags*/)
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(
	FAbilityEquipped,
	const FGameplayTag& /*AbilityTag*/,
	const FGameplayTag& /*StatusTag*/,
	const FGameplayTag& /*SlotTag*/,
	const FGameplayTag& /*PreviousSlotTag*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeactivatePassiveAbility, const FGameplayTag& /*PassiveAbilityTag*/)
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChangedDelegate;
	FAbilityEquipped AbilityEquippedDelegate;
	FOnDeactivatePassiveAbility OnDeactivatePassiveAbilityDelegate;

	bool bStartupAbilitiesGiven {false};

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& GrantedAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& GrantedPassiveAbilities);

	void AbilityInputTagPressed(const FGameplayTag InputTag);
	void AbilityInputTagHeld(const FGameplayTag InputTag);
	void AbilityInputTagReleased(const FGameplayTag InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	bool GetDescriptionsByAbilityTag(
		const FGameplayTag& AbilityTag,
		FString& OutDescription,
		FString& OutNextLevelDescription);
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetInputSlotTagFromAbilityTag(const FGameplayTag& AbilityTag);
	bool SlotIsEmpty(const FGameplayTag& SlotTag);
	static bool AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag);
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetAbilitySpecBySlotTag(const FGameplayTag& SlotTag);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;
	static void AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilitiesStatuses(int32 Level);

	UFUNCTION(Server, Reliable)
	void ServerUpdateAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoints(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void RequestServerForSpellAbilityInfo(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);
	
	static void ClearSlot(FGameplayAbilitySpec* AbilitySpec);
	void ClearAbilitiesOfSlot(const FGameplayTag& SlotTag);
	static bool HasAbilitySlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag);

	//------------------------------------------------------------------------//
	FAuraAbilityInfo SpellInfo = FAuraAbilityInfo();
	
protected:

	virtual void OnRep_ActivateAbilities() override;

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilitiesStatus(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		int32 AbilityLevel);
	
	UFUNCTION(Client, Reliable)
	void ClientUpdateSpellAbilityInfo(FAuraAbilityInfo InAbilityInfo);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(
		const FGameplayTag& AbilityTag,
		const FGameplayTag& StatusTag,
		const FGameplayTag& SlotTag,
		const FGameplayTag& PreviousSlotTag);
};