// Copyright - none


#include "AbilitySystem/Abilities/PickUpAbility.h"

#include "Combat/CombatComponent.h"

void UPickUpAbility::EquipItem(AActor* Item)
{
	AActor* AbilityOwner = GetAvatarActorFromActorInfo();
	if (!IsValid(AbilityOwner) || !AbilityOwner->Implements<UCombatComponent>())
	{
		return;
	}
}
