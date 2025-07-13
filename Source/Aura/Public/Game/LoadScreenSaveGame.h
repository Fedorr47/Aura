// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString SlotName{FString()};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 SlotIndex{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString PlayerName{FString("Default Name")};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus{Vacant};
};
