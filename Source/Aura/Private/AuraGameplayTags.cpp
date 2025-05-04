// Copyright - none


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// Secondary Attributes
	ADD_SECONDARY_TAG(Instance, Armor, "Reduces damage taken, improves Block Chance");
	ADD_SECONDARY_TAG(Instance, ArmorPenetration, "Ignores percentage of enemy Armor, increases Critical Hit Chance");
	ADD_SECONDARY_TAG(Instance, BlockChance, "Chance to cut incoming damage in half");
	ADD_SECONDARY_TAG(Instance, CriticalHitChance, "Chance to double damage plus critical hit bonus");
	ADD_SECONDARY_TAG(Instance, CriticalHitDamage, "Bonus damage added when a critical hit is scored");
	ADD_SECONDARY_TAG(Instance, CriticalHitResistance, "Reduces Critical Hit Chance of attacking enemies");
	ADD_SECONDARY_TAG(Instance, HealthRegeneration, "Amount of Health regenerated every 1 second");
	ADD_SECONDARY_TAG(Instance, ManaRegeneration, "Amount of Mana regenerated every 1 second");
	ADD_SECONDARY_TAG(Instance, MaxHealth, "Maximum amount of Health obtainable");
	ADD_SECONDARY_TAG(Instance, MaxMana, "Maximum amount of Mana obtainable");

	// Primary Attributes
	ADD_PRIMARY_TAG(Instance, Strength, "Increases physical damage");
	ADD_PRIMARY_TAG(Instance, Intelligence, "Increases magical damage");
	ADD_PRIMARY_TAG(Instance, Resilience, "Increases Armor and Armor Penetration");
	ADD_PRIMARY_TAG(Instance, Vigor, "Increases Health");

	// Vital Attributes
	ADD_VITAL_TAG(Instance, Health, "");
	ADD_VITAL_TAG(Instance, Mana, "");

	// Input Tags
	ADD_INPUT_TAG(Instance, LMB, "Input Tag for Left Mouse Button");
	ADD_INPUT_TAG(Instance, RMB, "Input Tag for Right Mouse Button");
	ADD_INPUT_TAG(Instance, 1, "Input Tag for 1 Key");
	ADD_INPUT_TAG(Instance, 2, "Input Tag for 2 Key");
	ADD_INPUT_TAG(Instance, 3, "Input Tag for 3 Key");
	ADD_INPUT_TAG(Instance, 4, "Input Tag for 4 Key");

	Instance.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"), FString("Damage"));
}
