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

	// Damage types
	ADD_DAMAGE_TAG(Instance, Fire, "Fire Damage Type");
	ADD_DAMAGE_TAG(Instance, Water, "Water Damage Type");
	ADD_DAMAGE_TAG(Instance, Air, "Air Damage Type");
	ADD_DAMAGE_TAG(Instance, Lightning, "Lightning Damage Type");
	ADD_DAMAGE_TAG(Instance, Arcane, "Arcane Damage Type");
	ADD_DAMAGE_TAG(Instance, Physical, "Physical Damage Type");

	// Damage Resistance types
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Fire, "Resistance to Fire Damage");
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Water, "Resistance to Water Damage");
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Air, "Resistance to Air Damage");
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Lightning, "Resistance to Lightning Damage");
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Arcane, "Resistance to Arcane Damage");
	ADD_DAMAGE_RESISTANCE_TAG(Instance, Physical, "Resistance to Physical Damage");

	// Abilities types
	ADD_ABILITIES_TAG(Instance, Attack, "Ability Attack");

	// Map of Damage types to Resistances
	Instance.DamageTypesToResistance.Add(Instance.Damage_Fire, Instance.Attribute_Resistance_Fire);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Air, Instance.Attribute_Resistance_Air);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Arcane, Instance.Attribute_Resistance_Arcane);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Water, Instance.Attribute_Resistance_Water);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Lightning, Instance.Attribute_Resistance_Lightning);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Physical, Instance.Attribute_Resistance_Physical);
	
	Instance.Effects_HitReact_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact.FireDamage"), FString("A hit reaction to fire damage"));
	Instance.Effects_HitReact_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact.PhysicalDamage"), FString("A hit reaction to Physical damage"));
	
	ADD_COMBAT_SOCKET_TAG(Instance, Weapon, "Weapon");
	ADD_COMBAT_SOCKET_TAG(Instance, LeftHand, "LeftHand");
	ADD_COMBAT_SOCKET_TAG(Instance, RightHand, "RightHand");

	ADD_MONTAGE_ATTACK_TAG(Instance, 1, "Attack 1");
	ADD_MONTAGE_ATTACK_TAG(Instance, 2, "Attack 2");
	ADD_MONTAGE_ATTACK_TAG(Instance, 3, "Attack 3");
	ADD_MONTAGE_ATTACK_TAG(Instance, 4, "Attack 4");
}
