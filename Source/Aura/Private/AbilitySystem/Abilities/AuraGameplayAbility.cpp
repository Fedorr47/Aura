// Copyright - none


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s </><Level>%d</>"), L"Default Ability Name - Default description", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</>\n<Default>Added something to somethings </>"),
		L"Default Ability", Level);
}

FString UAuraGameplayAbility::GetLockeDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level - </><Level>%i</>"), Level);
}