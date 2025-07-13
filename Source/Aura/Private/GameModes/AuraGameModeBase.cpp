// Copyright - none


#include "GameModes/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	if (LoadSaveGameClass != nullptr)
	{
 		USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadSaveGameClass);
		ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
		LoadScreenSaveGame->PlayerName = LoadSlot->PlayerName;
		LoadScreenSaveGame->SaveSlotStatus = ESaveSlotStatus::Taken;

		const bool res = UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
		if (!res)
		{
			UE_LOG(LogTemp, Warning, TEXT("Game wasn't saved = %s"));
		}
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex)
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadSaveGameClass);
	}
	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("LoadSaveGameClass = %s"), *GetNameSafe(LoadSaveGameClass));
}
