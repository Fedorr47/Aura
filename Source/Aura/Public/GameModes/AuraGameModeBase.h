// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UCharacterClassInfo;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> ListOfMaps;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag{FName()};
	
	//----------------------------------------------------------------------------------------------------------------//
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex);

	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	void LoadLevelBySlot(UMVVM_LoadSlot* LoadSlot);

	ULoadScreenSaveGame* RetrieveInGameSaveDatta();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;
};
