// Copyright - none


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

FAuraGameplayTags FAuraGameplayTags::Instance;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// Secondary Attributes
	Instance.Attribute_Secondary_Armor                 = UAuraAbilitySystemLibrary::AddInputTag("Reduces damage taken, improves Block Chance",         "Attributes", "Secondary", "Armor");
	Instance.Attribute_Secondary_ArmorPenetration      = UAuraAbilitySystemLibrary::AddInputTag("Ignores percentage of enemy Armor, increases Critical Hit Chance", "Attributes", "Secondary", "ArmorPenetration");
	Instance.Attribute_Secondary_BlockChance           = UAuraAbilitySystemLibrary::AddInputTag("Chance to cut incoming damage in half",               "Attributes", "Secondary", "BlockChance");
	Instance.Attribute_Secondary_CriticalHitChance     = UAuraAbilitySystemLibrary::AddInputTag("Chance to double damage plus critical hit bonus",     "Attributes", "Secondary", "CriticalHitChance");
	Instance.Attribute_Secondary_CriticalHitDamage     = UAuraAbilitySystemLibrary::AddInputTag("Bonus damage added when a critical hit is scored",   "Attributes", "Secondary", "CriticalHitDamage");
	Instance.Attribute_Secondary_CriticalHitResistance = UAuraAbilitySystemLibrary::AddInputTag("Reduces Critical Hit Chance of attacking enemies",    "Attributes", "Secondary", "CriticalHitResistance");
	Instance.Attribute_Secondary_HealthRegeneration    = UAuraAbilitySystemLibrary::AddInputTag("Amount of Health regenerated every 1 second",         "Attributes", "Secondary", "HealthRegeneration");
	Instance.Attribute_Secondary_ManaRegeneration      = UAuraAbilitySystemLibrary::AddInputTag("Amount of Mana regenerated every 1 second",           "Attributes", "Secondary", "ManaRegeneration");
	Instance.Attribute_Secondary_MaxHealth             = UAuraAbilitySystemLibrary::AddInputTag("Maximum amount of Health obtainable",                "Attributes", "Secondary", "MaxHealth");
	Instance.Attribute_Secondary_MaxMana               = UAuraAbilitySystemLibrary::AddInputTag("Maximum amount of Mana obtainable",                  "Attributes", "Secondary", "MaxMana");
	
	// Primary Attributes
	Instance.Attribute_Primary_Strength     = UAuraAbilitySystemLibrary::AddInputTag("Increases physical damage",             "Attributes", "Primary", "Strength");
	Instance.Attribute_Primary_Intelligence = UAuraAbilitySystemLibrary::AddInputTag("Increases magical damage",              "Attributes", "Primary", "Intelligence");
	Instance.Attribute_Primary_Resilience   = UAuraAbilitySystemLibrary::AddInputTag("Increases Armor and Armor Penetration", "Attributes", "Primary", "Resilience");
	Instance.Attribute_Primary_Vigor        = UAuraAbilitySystemLibrary::AddInputTag("Increases Health",                      "Attributes", "Primary", "Vigor");
	
	// Vital Attributes
	Instance.Attribute_Vital_Health = UAuraAbilitySystemLibrary::AddInputTag("Vital Health attribute", "Attributes", "Vital", "Health");
	Instance.Attribute_Vital_Mana   = UAuraAbilitySystemLibrary::AddInputTag("Vital Mana attribute",   "Attributes", "Vital", "Mana");
	
	// Resistances
	Instance.Attribute_Resistance_Fire      = UAuraAbilitySystemLibrary::AddInputTag("Resistance to fire",      "Attributes", "Resistance", "Fire");
	Instance.Attribute_Resistance_Water     = UAuraAbilitySystemLibrary::AddInputTag("Resistance to water",     "Attributes", "Resistance", "Water");
	Instance.Attribute_Resistance_Air       = UAuraAbilitySystemLibrary::AddInputTag("Resistance to air",       "Attributes", "Resistance", "Air");
	Instance.Attribute_Resistance_Lightning = UAuraAbilitySystemLibrary::AddInputTag("Resistance to lightning", "Attributes", "Resistance", "Lightning");
	Instance.Attribute_Resistance_Arcane    = UAuraAbilitySystemLibrary::AddInputTag("Resistance to arcane",    "Attributes", "Resistance", "Arcane");
	Instance.Attribute_Resistance_Physical  = UAuraAbilitySystemLibrary::AddInputTag("Resistance to physical",  "Attributes", "Resistance", "Physical");

	// Debuffs
	Instance.Debuff_Burn	= UAuraAbilitySystemLibrary::AddInputTag("Debuff for fire damage",      "Debuff", "Burn");
	Instance.Debuff_Wet		= UAuraAbilitySystemLibrary::AddInputTag("Debuff for water damage",     "Debuff", "Wet");
	Instance.Debuff_Arcane	= UAuraAbilitySystemLibrary::AddInputTag("Debuff for arcane damage",    "Debuff", "Arcane");
	Instance.Debuff_Criple	= UAuraAbilitySystemLibrary::AddInputTag("Debuff for physical damage",	"Debuff", "Criple");
	Instance.Debuff_Shock	= UAuraAbilitySystemLibrary::AddInputTag("Debuff for lightning damage", "Debuff", "Shock");
	Instance.Debuff_Dry		= UAuraAbilitySystemLibrary::AddInputTag("Debuff for air damage",		"Debuff", "Dry");

	// Debuff props		
	Instance.Debuff_Chance		= UAuraAbilitySystemLibrary::AddInputTag("Debuff Chance",		"Debuff", "Chance");
	Instance.Debuff_Damage		= UAuraAbilitySystemLibrary::AddInputTag("Debuff Damage",		"Debuff", "Damage");
	Instance.Debuff_Duration	= UAuraAbilitySystemLibrary::AddInputTag("Debuff Duration",		"Debuff", "Duration");
	Instance.Debuff_Frequency	= UAuraAbilitySystemLibrary::AddInputTag("Debuff Frequency",	"Debuff", "Frequency");
	
	// Meta Attribute
	Instance.Attribute_Meta_XP = UAuraAbilitySystemLibrary::AddInputTag("Meta Attribute Incoming XP", "Attributes", "Meta", "XP");

	// Input Tags
	Instance.InputTag	  = UAuraAbilitySystemLibrary::AddInputTag("Default Input Tag", "InputTag");
	Instance.InputTag_LMB = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for Left Mouse Button", "InputTag", "LMB");
	Instance.InputTag_RMB = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for Right Mouse Button", "InputTag", "RMB");
	Instance.InputTag_1   = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for 1 Key", "InputTag", "1");
	Instance.InputTag_2   = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for 2 Key", "InputTag", "2");
	Instance.InputTag_3   = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for 3 Key", "InputTag", "3");
	Instance.InputTag_4   = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for 4 Key", "InputTag", "4");
	Instance.InputTag_Passive_1 = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for Passive 1", "InputTag", "Passive", "1");
	Instance.InputTag_Passive_2 = UAuraAbilitySystemLibrary::AddInputTag("Input Tag for Passive 2", "InputTag", "Passive", "2");

	// Base tag
	Instance.Damage = UAuraAbilitySystemLibrary::AddInputTag("Damage", "Damage");
	
	// Damage types
	Instance.Damage_Fire     = UAuraAbilitySystemLibrary::AddInputTag("Fire Damage Type",     "Damage", "Fire");
	Instance.Damage_Water    = UAuraAbilitySystemLibrary::AddInputTag("Water Damage Type",    "Damage", "Water");
	Instance.Damage_Air      = UAuraAbilitySystemLibrary::AddInputTag("Air Damage Type",      "Damage", "Air");
	Instance.Damage_Lightning= UAuraAbilitySystemLibrary::AddInputTag("Lightning Damage Type","Damage", "Lightning");
	Instance.Damage_Arcane   = UAuraAbilitySystemLibrary::AddInputTag("Arcane Damage Type",   "Damage", "Arcane");
	Instance.Damage_Physical = UAuraAbilitySystemLibrary::AddInputTag("Physical Damage Type", "Damage", "Physical");
	
	// Ability types
	Instance.Abilities_None = UAuraAbilitySystemLibrary::AddInputTag("Ability None", "Abilities", "None");
	Instance.Abilities_Attack = UAuraAbilitySystemLibrary::AddInputTag("Ability Attack", "Abilities", "Attack");
	Instance.Abilities_Summon = UAuraAbilitySystemLibrary::AddInputTag("Ability Summon", "Abilities", "Summon");
	
	// Damage → Resistance map
	Instance.DamageTypesToResistance.Add(Instance.Damage_Fire,     Instance.Attribute_Resistance_Fire);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Water,    Instance.Attribute_Resistance_Water);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Air,      Instance.Attribute_Resistance_Air);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Lightning,Instance.Attribute_Resistance_Lightning);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Arcane,   Instance.Attribute_Resistance_Arcane);
	Instance.DamageTypesToResistance.Add(Instance.Damage_Physical, Instance.Attribute_Resistance_Physical);

	// Damage → Debuffs
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Fire,		Instance.Debuff_Burn);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Water,		Instance.Debuff_Wet);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Arcane,		Instance.Debuff_Arcane);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Physical,	Instance.Debuff_Criple);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Lightning,	Instance.Debuff_Shock);
	Instance.DamageTypesToDebuffs.Add(Instance.Damage_Air,		Instance.Debuff_Dry);
	
	// Effects (Hit React)
	Instance.Effects_HitReact_Fire     = UAuraAbilitySystemLibrary::AddInputTag("A hit reaction to fire damage",     "Effects", "HitReact", "FireDamage");
	Instance.Effects_HitReact_Physical = UAuraAbilitySystemLibrary::AddInputTag("A hit reaction to Physical damage", "Effects", "HitReact", "PhysicalDamage");
	
	// Combat Sockets
	Instance.CombatSocket_Weapon    = UAuraAbilitySystemLibrary::AddInputTag("Weapon",    "CombatSocket", "Weapon");
	Instance.CombatSocket_LeftHand  = UAuraAbilitySystemLibrary::AddInputTag("LeftHand",  "CombatSocket", "LeftHand");
	Instance.CombatSocket_RightHand = UAuraAbilitySystemLibrary::AddInputTag("RightHand", "CombatSocket", "RightHand");
	
	// Montage Attack tags
	Instance.Montage_Attack_1 = UAuraAbilitySystemLibrary::AddInputTag("Attack 1", "Montage", "Attack", "1");
	Instance.Montage_Attack_2 = UAuraAbilitySystemLibrary::AddInputTag("Attack 2", "Montage", "Attack", "2");
	Instance.Montage_Attack_3 = UAuraAbilitySystemLibrary::AddInputTag("Attack 3", "Montage", "Attack", "3");
	Instance.Montage_Attack_4 = UAuraAbilitySystemLibrary::AddInputTag("Attack 4", "Montage", "Attack", "4");
	
	// Abilities.Fire.*
	Instance.Abilities_Fire_FireBolt = UAuraAbilitySystemLibrary::AddInputTag("Fire Bolt Ability Tag", "Abilities", "Fire", "FireBolt");
	Instance.Abilities_Fire_FireBlast = UAuraAbilitySystemLibrary::AddInputTag("Fire Blast Ability Tag", "Abilities", "Fire", "FireBlast");
	// Abilities.Lightning.*
	Instance.Abilities_Lightning_Electrocute = UAuraAbilitySystemLibrary::AddInputTag("Electrocute Ability Tag", "Abilities", "Lightning", "Electrocute");
	// Abilities.Arcane.*
	Instance.Abilities_Arcane_ArcaneShards = UAuraAbilitySystemLibrary::AddInputTag("ArcaneShards Ability Tag", "Abilities", "Arcane", "ArcaneShards");

	// Passive abilities
	Instance.Abilities_Passive_Protection = UAuraAbilitySystemLibrary::AddInputTag("Protection", "Abilities", "Passive", "Protection");
	Instance.Abilities_Passive_LifeSiphon = UAuraAbilitySystemLibrary::AddInputTag("Life Siphon", "Abilities", "Passive", "LifeSiphon");
	Instance.Abilities_Passive_ManaSiphon = UAuraAbilitySystemLibrary::AddInputTag("Mana Siphon", "Abilities", "Passive", "ManaSiphon");
	
	// Cooldown.Fire.FireBolt
	Instance.Cooldown_Fire_FireBolt = UAuraAbilitySystemLibrary::AddInputTag("Fire Bolt Cooldown Tag", "Cooldown", "Fire", "FireBolt");
	Instance.Cooldown_Lightning_Electrocute = UAuraAbilitySystemLibrary::AddInputTag("Electrocute Cooldown Tag", "Cooldown", "Lightning", "Electrocute");
	Instance.Cooldown_Arcane_ArcaneShards = UAuraAbilitySystemLibrary::AddInputTag("Arcane Shards Cooldown Tag", "Cooldown", "Arcane", "ArcaneShards");
	Instance.Cooldown_Fire_FireBlast = UAuraAbilitySystemLibrary::AddInputTag("Fire Blast Cooldown Tag", "Cooldown", "Fire", "FireBlast");

	// Abilities Statuses
	Instance.Abilities_Status_Locked = UAuraAbilitySystemLibrary::AddInputTag("Ability status Locked", "Abilities", "Status", "Locked");
	Instance.Abilities_Status_Eligible = UAuraAbilitySystemLibrary::AddInputTag("Ability status Eligible", "Abilities", "Status", "Eligible");
	Instance.Abilities_Status_Unlocked = UAuraAbilitySystemLibrary::AddInputTag("Ability status Unlocked", "Abilities", "Status", "Unlocked");
	Instance.Abilities_Status_Equipped = UAuraAbilitySystemLibrary::AddInputTag("Ability status Equipped", "Abilities", "Status", "Equipped");

	Instance.Abilities_Type_Offensive = UAuraAbilitySystemLibrary::AddInputTag("Ability Type Offensive", "Abilities", "Type", "Offensive");
	Instance.Abilities_Type_Passive = UAuraAbilitySystemLibrary::AddInputTag("Ability Type Passive", "Abilities", "Type", "Passive");
	Instance.Abilities_Type_None = UAuraAbilitySystemLibrary::AddInputTag("Ability Type None", "Abilities", "Type", "None");

	// Player block states
	Instance.Player_Block_InputPressed	= UAuraAbilitySystemLibrary::AddInputTag("Block input Pressed callback for input", "Player", "Block", "InputPressed");
	Instance.Player_Block_InputHeld		= UAuraAbilitySystemLibrary::AddInputTag("Block input Held callback for input", "Player", "Block", "InputHeld");
	Instance.Player_Block_InputReleased	= UAuraAbilitySystemLibrary::AddInputTag("Block input Released callback for input", "Player", "Block", "InputReleased");
	Instance.Player_Block_CursorTrace	= UAuraAbilitySystemLibrary::AddInputTag("Block tracing under the cursor", "Player", "Block", "CursorTrace");

	// GameplayCues
	Instance.GameplayCue_FireBlast = UAuraAbilitySystemLibrary::AddInputTag("FireBlast GameplayCue Tag", "GameplayCue", "FireBlast");
}
