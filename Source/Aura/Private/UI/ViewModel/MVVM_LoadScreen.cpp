// Copyright - none


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "GameModes/AuraGameModeBase.h"
#include "Game/AuraGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->LoadSlotIndex = 0;
	LoadSlotView.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->LoadSlotIndex = 1;
	LoadSlotView.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->LoadSlotIndex = 2;
	LoadSlotView.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlotView.Num());
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlotView.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 SlotIndex, FString& SlotName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	
	if (!IsValid(AuraGameMode))
	{
		return;
	}
	
	LoadSlotView[SlotIndex]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlotView[SlotIndex]->SetPlayerName(SlotName);
	LoadSlotView[SlotIndex]->SetPlayerLevel(1);
	LoadSlotView[SlotIndex]->SlotStatus = ESaveSlotStatus::Taken;
	LoadSlotView[SlotIndex]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	
	AuraGameMode->SaveSlotData(LoadSlotView[SlotIndex], SlotIndex);
	LoadSlotView[SlotIndex]->InitializeSlot();

	if (UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance()))
	{
		AuraGameInstance->LoadSlotName = LoadSlotView[SlotIndex]->GetLoadSlotName();
		AuraGameInstance->LoadSlotIndex = LoadSlotView[SlotIndex]->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlotView[SlotIndex]->OnSetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	OnSlotSelectedDelegate.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*>& InLoadSlotPair : LoadSlotView)
	{
		InLoadSlotPair.Value->OnEnableSelectSlotButtonDelegate.Broadcast(SlotIndex != InLoadSlotPair.Key);
	}
	SelectedSlot = LoadSlotView[SlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->LoadSlotIndex);
		SelectedSlot->SlotStatus = ESaveSlotStatus::Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->OnEnableSelectSlotButtonDelegate.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameModeBase) || !IsValid(SelectedSlot))
	{
		return;
	}
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameModeBase->GetGameInstance());
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->LoadSlotIndex;
	
	AuraGameModeBase->LoadLevelBySlot(SelectedSlot);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameModeBase))
	{
		return;
	}
	for (const TTuple<int32, UMVVM_LoadSlot*>& InLoadSlotPair : LoadSlotView)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameModeBase->GetSaveSlotData(InLoadSlotPair.Value->GetLoadSlotName(), InLoadSlotPair.Key);
		const FString lPlayerName = SaveObject->PlayerName;

		InLoadSlotPair.Value->SlotStatus = SaveObject->SaveSlotStatus;
		InLoadSlotPair.Value->SetPlayerName(lPlayerName);
		InLoadSlotPair.Value->InitializeSlot();
		
		InLoadSlotPair.Value->SetMapName(SaveObject->MapName);
		InLoadSlotPair.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		InLoadSlotPair.Value->SetPlayerLevel(SaveObject->PlayerLevel);
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
