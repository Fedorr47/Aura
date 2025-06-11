// Copyright - none


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ARCANE SHARDS:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Cast a </>"
		  "<Damage> %i</>"
		  "<Default> of shards that damage characters maximum by</>"
		  "<Damage> %.1f</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, MaxShardPoints),
					   ScaledDamage
					   );
}

FString UAuraArcaneShards::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Cast a </>"
		  "<Damage> %i</>"
		  "<Default> of shards that damage characters maximum by</>"
		  "<Damage> %.1f/>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, MaxShardPoints),
					   ScaledDamage
					   );
}
