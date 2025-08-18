// Copyright - none


#include "Cheats/DebugCheatManager.h"

void UDebugCheatManager::God()
{
	Super::God();
}

void UDebugCheatManager::KillAll(const FString& ClassName)
{
}

void UDebugCheatManager::UnlimitedMana()
{
	if (APawn* OwnerPawn = GetOwnerPawn())
	{
		//OwnerPawn->
	}
}

APawn* UDebugCheatManager::GetOwnerPawn()
{
	APawn* Pawn = GetOuterAPlayerController()->GetPawn();
	return Pawn;
}
