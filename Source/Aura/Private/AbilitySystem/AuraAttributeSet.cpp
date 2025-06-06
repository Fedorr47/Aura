// Copyright - none


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Primary, Strength);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Primary, Intelligence);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Primary, Resilience);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Primary, Vigor);
	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, Armor);				
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, ArmorPenetration);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, BlockChance);		
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, CriticalHitChance);	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, CriticalHitDamage);	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, CriticalHitResistance);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, HealthRegeneration);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, ManaRegeneration);	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, MaxHealth);	
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Secondary, MaxMana);

	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Vital, Health);
	BIND_TAGS_WITH_FUNC(TagsToAttributes, GameplayTags, Vital, Mana);

	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Fire);
	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Air);
	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Arcane);
	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Lightning);
	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Physical);
	BIND_TAGS_RESISTANCE_WITH_FUNC(TagsToAttributes, GameplayTags, Resistance, Water);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	// Secondary attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, 		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration,		COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration,			COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana,					COND_None, REPNOTIFY_Always);
	// Vital attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health,	COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	// Resistance
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, WaterResistance,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, AirResistance,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance,					COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance,				COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance,				COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	CLAMP_ATTRIBUTE(Health, Attribute, 0.0f, GetMaxHealth(), NewValue);
	CLAMP_ATTRIBUTE(Health, Attribute, 0.0f, GetMaxMana(), NewValue);
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Props.TargetCharacter))
		{
			return;
		}
	}
	
	const auto Attribute = Data.EvaluatedData.Attribute;
	CLAMP_ATTRIBUTE_POST(Health, Attribute, 0.0f, GetMaxHealth());
	CLAMP_ATTRIBUTE_POST(Mana, Attribute, 0.0f, GetMaxMana());

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute == GetIncomingExperiencePointsAttribute())
	{
		HandleIncomingExperiencePoints(Props);
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.0f);
	if (LocalIncomingDamage > 0.0f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.0f;
		if (bFatal)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendExperiencePoints(Props);
		}
		else
		{
			const FGameplayEffectContextHandle& EffectContextHandle = Props.EffectContextHandle;
			FGameplayTagContainer TagHitReactContainer;
			if (const FGameplayEffectContext* Context = EffectContextHandle.Get())
			{
				if (const UAuraDamageGameplayAbility* AuraDamageGameplayAbility = Cast<UAuraDamageGameplayAbility>(Context->GetAbility()))
				{
					TagHitReactContainer.AddTag(AuraDamageGameplayAbility->HitReactType);
				}
			}
			if (TagHitReactContainer.Num() > 0)
			{
				Props.TargetASC->TryActivateAbilitiesByTag(TagHitReactContainer);
			}
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Knockback(UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle));
			}
		}

		const bool bBlockHit = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlockHit, bCriticalHit);
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			HandleDebuff(Props);
		}
	}
}

void UAuraAttributeSet::HandleDebuff(const FEffectProperties& Props)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
	TagContainer.Added.AddTag(DebuffTag);
	TagContainer.CombinedTags.AddTag(DebuffTag);
	/*
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Shock))
	{
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
	}*/
	Component.SetAndApplyTargetTagChanges(TagContainer);
	//Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContextHandle, 1.0f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageTag = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageTag);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UAuraAttributeSet::HandleIncomingExperiencePoints(const FEffectProperties& Props)
{
	const float LocalIncomingExperiencePoints = GetIncomingExperiencePoints();
	SetIncomingExperiencePoints(0.0f);
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentExperiencePoints = IPlayerInterface::Execute_GetExperiencePoints(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForExperiencePoints(
			Props.SourceCharacter,
			CurrentExperiencePoints + LocalIncomingExperiencePoints);
		const int32 NumLevelsUp = NewLevel - CurrentLevel;

		if (NumLevelsUp > 0)
		{
			int32 AttributesPointRewards = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel);
			int32 SpellPointRewards = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel);

			IPlayerInterface::Execute_AddPlayerLevel(Props.SourceCharacter, NumLevelsUp);
			IPlayerInterface::Execute_AddAttributePoints(Props.SourceCharacter, AttributesPointRewards);
			IPlayerInterface::Execute_AddSpellPoints(Props.SourceCharacter, SpellPointRewards);

			SetHealth(GetMaxHealth());
			SetMana(GetMaxMana());

			bTopOffHealth = true;
			bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToExperiencePoints(Props.SourceCharacter, LocalIncomingExperiencePoints);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit)
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumbers(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumbers(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UAuraAttributeSet::SendExperiencePoints(const FEffectProperties& Props)
{
	int32 RewardAmount = UAuraAbilitySystemLibrary::GetReward(
		Props.TargetCharacter,
		ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter),
		ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter));

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEventData Payload;
	Payload.EventTag = GameplayTags.Attribute_Meta_XP;
	Payload.EventMagnitude = RewardAmount;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Props.SourceCharacter,
		GameplayTags.Attribute_Meta_XP,
		Payload);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetCharacter());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

/*
 * Primary attributes replication
 */
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

/*
 * Secondary attributes replication
 */

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

/*
 * Vital attributes replication
 */
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

/*
 *  Resistance attributes replication
 */

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_WaterResistance(const FGameplayAttributeData& OldWaterResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, WaterResistance, OldWaterResistance);
}

void UAuraAttributeSet::OnRep_AirResistance(const FGameplayAttributeData& OldAirResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AirResistance, OldAirResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}
