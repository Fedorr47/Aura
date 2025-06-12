// Copyright - none


#include "AbilitySystem/Data/CharacterClassInfo.h"

// TODO: Crash here
FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
