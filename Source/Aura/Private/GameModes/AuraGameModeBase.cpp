// Copyright - none


#include "GameModes/AuraGameModeBase.h"

#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	if (LoadSaveGameClass != nullptr)
	{
 		USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadSaveGameClass);
		ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
		LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
		LoadScreenSaveGame->SaveSlotStatus = ESaveSlotStatus::Taken;
		LoadScreenSaveGame->MapName = LoadSlot->GetMapName();

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

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::LoadLevelBySlot(UMVVM_LoadSlot* LoadSlot)
{
	const FString SlotName = LoadSlot->GetLoadSlotName();
	const int32 SlotIndex = LoadSlot->LoadSlotIndex;
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot,ListOfMaps.FindChecked(LoadSlot->GetMapName()));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	ListOfMaps.Add(DefaultMapName, DefaultMap);
}
