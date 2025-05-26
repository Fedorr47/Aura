// Copyright - none


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>FIRE BOLT:</>\n\n<Default>Launches %i bolts of fire, exploding on impact and dealing</><Damage> %d </><Default>fire damage with a chance to burn</>\n\n<Default>Level:</><Level> %d</>"),
					   FMath::Min(Level, NumOfProjectiles), ScaledDamage, Level);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL:</>\n\n<Default>Launches %i bolts of fire, exploding on impact and dealing</><Damage> %d </><Default>fire damage with a chance to burn</>\n\n<Default>Level:</><Level> %d</>"),
					   FMath::Min(Level, NumOfProjectiles), ScaledDamage, Level);
}
