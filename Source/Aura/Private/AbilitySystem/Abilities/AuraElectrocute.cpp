// Copyright - none


#include "AbilitySystem/Abilities/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>ELECTROCUTE:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Cast a electric beam that damage by </>"
		  "<Damage> %.1f </>"
		  "<Default>repeats on </>"
		  "<Damage> - %i</>"
		  "<Default> enemies</>"
		"<Default>lightning damage with a chance to stun</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   ScaledDamage,
					   FMath::Min(Level, MaxNumShockTargets)
					   );
}

FString UAuraElectrocute::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		"<Default>Cast a electric beam that damage by </>"
		 "<Damage> %.1f </>"
		 "<Default>repeats on </>"
		 "<Damage> - %i</>"
		 "<Default> enemies</>"
		"<Default>lightning damage with a chance to stun</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   ScaledDamage,
					   FMath::Min(Level, MaxNumShockTargets)
					   );
}
