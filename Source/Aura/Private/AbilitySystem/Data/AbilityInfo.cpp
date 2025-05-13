// Copyright - none


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound /*= false*/)
{
	for (const FAuraAbilityInfo& AbilityInfo : AbilitiyInformation)
	{
		if (AbilityInfo.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("No such ability info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAbilityInfo();
}

