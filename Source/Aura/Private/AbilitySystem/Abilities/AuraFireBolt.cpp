// Copyright - none


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
  		"<Default>Launches %i bolts of fire, exploding on impact and dealing</>"
  		"<Damage> %d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfProjectiles),
					   ScaledDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Launches %i bolts of fire, exploding on impact and dealing</>"
		  "<Damage> %d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfProjectiles),
					   ScaledDamage);
}
