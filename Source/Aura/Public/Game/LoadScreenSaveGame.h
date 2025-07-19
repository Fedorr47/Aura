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
	FString PlayerName{FString()};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FString MapName{FString()};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	FName PlayerStartTag{FName()};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus{Vacant};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bFirstTimeLoading{true};

	/* Player */
	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 PlayerLevel{1};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 ExperiencePoints{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 SpellPoints{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	int32 AttributePoints{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float Strength{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float Intelligence{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float Resilience{0};

	UPROPERTY(BlueprintReadWrite, SaveGame)
	float Vigor{0};;
	/* End Player */
};
