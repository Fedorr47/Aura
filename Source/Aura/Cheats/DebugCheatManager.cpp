// Copyright - none


#include "Cheats/DebugCheatManager.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/AuraCharacter.h"
#include "Interaction/CheatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerState.h"

void UDebugCheatManager::God()
{
	Super::God();
}

void UDebugCheatManager::KillAll(const FString& ClassName)
{
}

void UDebugCheatManager::UnlimitedMana(bool NewValue)
{
	if (APawn* OwnerPawn = GetOwnerPawn())
	{
		if (OwnerPawn->Implements<UPlayerInterface>())
		{
			if (auto* PS =GetOuterAPlayerController()->GetPlayerState<AAuraPlayerState>())
			{
				ICheatInterface::Execute_SetUnlimitedMana(PS, NewValue);
				if (!OwnerPawn->HasAuthority())
				{
					PS->Server_SetUnlimitedMana(NewValue);
				}
			}
		}
	}
}

APawn* UDebugCheatManager::GetOwnerPawn()
{
	APawn* Pawn = GetOuterAPlayerController()->GetPawn();
	return Pawn;
}
