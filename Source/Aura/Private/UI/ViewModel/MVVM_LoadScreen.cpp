// Copyright - none


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "GameModes/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlotView.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlotView.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
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
	
	LoadSlotView[SlotIndex]->SetPlayerName(SlotName);
	LoadSlotView[SlotIndex]->SlotStatus = ESaveSlotStatus::Taken;
	
	AuraGameMode->SaveSlotData(LoadSlotView[SlotIndex], SlotIndex);
	LoadSlotView[SlotIndex]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 SlotIndex)
{
	LoadSlotView[SlotIndex]->OnSetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	for (const TTuple<int32, UMVVM_LoadSlot*>& InLoadSlotPair : LoadSlotView)
	{
		InLoadSlotPair.Value->OnEnableSelectSlotButtonDelegate.Broadcast(SlotIndex != InLoadSlotPair.Key);
	}
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
	}
}

void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
