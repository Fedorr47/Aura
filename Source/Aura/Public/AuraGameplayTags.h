// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#define ADD_SECONDARY_TAG(Instance, AttributeName, TagComment) \
Instance.Attribute_Secondary_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Attributes.Secondary."#AttributeName), FString(TagComment))

#define ADD_PRIMARY_TAG(Instance, AttributeName, TagComment) \
Instance.Attribute_Primary_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Attributes.Primary."#AttributeName), FString(TagComment))

#define ADD_VITAL_TAG(Instance, AttributeName, TagComment) \
Instance.Attribute_Vital_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Attributes.Vital."#AttributeName), FString(TagComment))

#define ADD_META_TAG(Instance, AttributeName, TagComment) \
Instance.Attribute_Meta_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Attributes.Meta."#AttributeName), FString(TagComment))

#define ADD_INPUT_TAG(Instance, InputName, TagComment) \
Instance.InputTag_##InputName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("InputTag."#InputName), FString(TagComment))

#define ADD_DAMAGE_TAG(Instance, AttributeName, TagComment) \
Instance.Damage_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Damage."#AttributeName), FString(TagComment))

#define ADD_DAMAGE_RESISTANCE_TAG(Instance, AttributeName, TagComment) \
Instance.Attribute_Resistance_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Attributes.Resistance."#AttributeName), FString(TagComment))

#define ADD_ABILITY_TAG(Instance, AttributeName, TagComment) \
Instance.Abilities_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Abilities."#AttributeName), FString(TagComment))

#define ADD_MONTAGE_ATTACK_TAG(Instance, AttributeName, TagComment) \
Instance.Montage_Attack_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("Montage.Attack"#AttributeName), FString(TagComment))

#define ADD_COMBAT_SOCKET_TAG(Instance, AttributeName, TagComment) \
Instance.CombatSocket_##AttributeName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName("CombatSocket."#AttributeName), FString(TagComment))

#define ADD_ABILITY_TYPE_TAG(Instance, AbilityType, AbilityName, TagComment) \
Instance.Abilities_##AbilityType##_##AbilityName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName(TEXT("Abilities." #AbilityType "." #AbilityName)), FString(TagComment))

#define ADD_ABILITY_COOLDOWN_TYPE_TAG(Instance, AbilityType, AbilityName, TagComment) \
Instance.Cooldown_##AbilityType##_##AbilityName = UGameplayTagsManager::Get().AddNativeGameplayTag( \
FName(TEXT("Cooldown." #AbilityType "." #AbilityName)), FString(TagComment))

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get()
	{
		return Instance;
	}

	static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
	
	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;

	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Water;
	FGameplayTag Attribute_Resistance_Air;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;
	
	FGameplayTag Attribute_Meta_XP;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Water;
	FGameplayTag Damage_Air;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;

	FGameplayTag Effects_HitReact_Fire;
	FGameplayTag Effects_HitReact_Physical;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Weapon;

	FGameplayTag Abilities_Fire_FireBolt;
	// Cooldown
	FGameplayTag Cooldown_Fire_FireBolt;

protected:
private:
	static FAuraGameplayTags Instance;
	
	FAuraGameplayTags() = default;
	~FAuraGameplayTags() = default;
};
