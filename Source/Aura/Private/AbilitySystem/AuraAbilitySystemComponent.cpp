// Copyright - none


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& GrantedAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : GrantedAbilities)
	{
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& GrantedPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : GrantedPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Type_Passive);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				UGameplayAbility* AbilityInstance = AbilitySpec.GetPrimaryInstance();
				if (AbilityInstance)
				{
					FPredictionKey PredictionKey = AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey();

					InvokeReplicatedEvent(
						EAbilityGenericReplicatedEvent::InputReleased,
						AbilitySpec.Handle,
						PredictionKey);
				}
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
				UE_LOG(LogAura, Log, TEXT("Ability %s"), *InputTag.ToString());
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			UGameplayAbility* AbilityInstance = AbilitySpec.GetPrimaryInstance();
			if (AbilityInstance)
			{
				FPredictionKey PredictionKey = AbilityInstance->GetCurrentActivationInfo().GetActivationPredictionKey();

				InvokeReplicatedEvent(
					EAbilityGenericReplicatedEvent::InputReleased,
					AbilitySpec.Handle,
					PredictionKey);
			}
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopedLock(*this); // Like a lock_guard
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	RequestServerForSpellAbilityInfo(AbilityTag);
	OutDescription = FString();
	if (AbilityTag.IsValid() && !AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = UAuraGameplayAbility::GetLockeDescription(SpellInfo.LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag AbilityTag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return AbilityTag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag InputTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (InputTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return InputTag;
		}
	}

	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}

	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->GetAssetTags())
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputSlotTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

inline bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(SlotTag);
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& AbilitySpec)
{
	return AbilitySpec.GetDynamicSpecSourceTags().HasTag(FAuraGameplayTags::Get().InputTag);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetAbilitySpecBySlotTag(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(SlotTag))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	return Info.AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive);
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
	ClearSlot(&AbilitySpec);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(SlotTag);
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec,SlotTag))
		{
			return false;
		}
	}
	return true;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (const int32 AttributePoints = IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()); AttributePoints > 0)
		{
			ServerUpdateAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilitiesStatuses(int32 Level)
{
	UAbilityInfo* AbilitiesInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiesInfo->AbilitiyInformation)
	{
		if (!AbilityInfo.AbilityTag.IsValid() || Level < AbilityInfo.LevelRequirement)
		{
			continue;
		}
		if (GetSpecFromAbilityTag(AbilityInfo.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityInfo.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilitiesStatus(
				AbilityInfo.AbilityTag,
				FAuraGameplayTags::Get().Abilities_Status_Eligible,
				1);
		}
	}
}

void UAuraAbilitySystemComponent::MulticastActivePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bIsAlreadyActive)
{
	OnActivatePassiveEffectDelegate.Broadcast(AbilityTag, bIsAlreadyActive);
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag SlotTag = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(SlotTag);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (HasAbilitySlot(&AbilitySpec, SlotTag))
		{
			ClearSlot(&AbilitySpec);
		}
	}
}

bool UAuraAbilitySystemComponent::HasAbilitySlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag AbilityTag : AbilitySpec->GetDynamicSpecSourceTags())
	{
		if (AbilityTag.MatchesTagExact(SlotTag))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::RequestServerForSpellAbilityInfo_Implementation(const FGameplayTag& AbilityTag)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	ClientUpdateSpellAbilityInfo(Info);
}

void UAuraAbilitySystemComponent::ServerUpdateAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.0f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetAvatarActor(),
		AttributeTag,
		Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoints_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddSpellPoints(GetAvatarActor(), -1);
		}
		
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) ||
			Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
		{
			++AbilitySpec->Level;
		}
		ClientUpdateAbilitiesStatus(
				AbilityTag,
				Status,
				AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FGameplayTag& PreviousSlotTag = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped ||
			Status == FAuraGameplayTags::Get().Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// Handle Activation/Deactivation for passive abilities
			if (!SlotIsEmpty(SlotTag))
			{
				FGameplayAbilitySpec* PrevAbility = GetAbilitySpecBySlotTag(SlotTag);
				if (PrevAbility)
				{
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*PrevAbility)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotTag, PreviousSlotTag);
						return;
					}
					if (IsPassiveAbility(*PrevAbility))
					{
						MulticastActivePassiveEffect(GetAbilityTagFromSpec(*PrevAbility), false);
						OnDeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*PrevAbility));
					}

					ClearSlot(PrevAbility);
				}
			}
			
			if (!AbilityHasAnySlot(*AbilitySpec))
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivePassiveEffect(AbilityTag, true);
				}
			}
			AssignSlotToAbility(*AbilitySpec, SlotTag);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotTag, PreviousSlotTag);
	}
}

void UAuraAbilitySystemComponent::ClientUpdateSpellAbilityInfo_Implementation(FAuraAbilityInfo inAbilityInfo)
{
	SpellInfo = inAbilityInfo;
}

void UAuraAbilitySystemComponent::ClientUpdateAbilitiesStatus_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec,
	FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag,
	const FGameplayTag& PreviousSlotTag)
{
	AbilityEquippedDelegate.Broadcast(
		AbilityTag,
		StatusTag,
		SlotTag,
		PreviousSlotTag);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}
