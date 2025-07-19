// Copyright - none


#include "GameModes/AuraGameModeBase.h"

#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
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
		LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

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

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	FName lPlayerStartTag("PlayerStart");
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		lPlayerStartTag = AuraGameInstance->PlayerStartTag;
	}
	
	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundPlayers);
	
	for (AActor* pActor : FoundPlayers)
	{
		if (APlayerStart* pPlayerStart = Cast<APlayerStart>(pActor))
		{
			if (pPlayerStart->PlayerStartTag == lPlayerStartTag)
			{
				return pPlayerStart;
			}
		}
	}
	return FoundPlayers.Num() > 0 ? FoundPlayers[0] : nullptr;
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveDatta()
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameSlotName = AuraGameInstance->LoadSlotName;
		const int32 SlotIndex = AuraGameInstance->LoadSlotIndex;

		return GetSaveSlotData(InGameSlotName, SlotIndex);
	}

	return nullptr;
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const FString InGameSlotName = AuraGameInstance->LoadSlotName;
		const int32 SlotIndex = AuraGameInstance->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

		UGameplayStatics::SaveGameToSlot(SaveObject, InGameSlotName, SlotIndex);
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	ListOfMaps.Add(DefaultMapName, DefaultMap);
}
