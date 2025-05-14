// Copyright - none


#include "AbilitySystem/Data/LevelUpInfo.h"

#include "Aura/AuraLogChannels.h"

int32 ULevelUpInfo::FindLevelUpInfoByXP(int32 InXP) const
{
	const auto Index = Algo::UpperBound(LevelUpInformation, InXP,
		[](int32 XP, const FAuraLevelUpInfo& Info)
		{
			return XP < Info.LevelUpRequirement;
		});
	
	return Index + 1;
}
