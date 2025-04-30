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


/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */
struct FAuraGameplayTags
{
public:
	FAuraGameplayTags(const FAuraGameplayTags&) = delete;
	FAuraGameplayTags& operator=(const FAuraGameplayTags&) = delete;
	
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

protected:
private:
	static FAuraGameplayTags Instance;
	
	FAuraGameplayTags() = default;
	~FAuraGameplayTags() = default;
};
