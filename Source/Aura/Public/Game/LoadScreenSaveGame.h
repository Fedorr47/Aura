// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(Blueprintable)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName{FName()};

	UPROPERTY()
	FTransform Transform{FTransform()};

	// Serialized variables from the Actor (marked by SaveGame)
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& lhs, const FSavedActor& rhs)
{
	return lhs.ActorName == rhs.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapAssetName{FString()};

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Class Defaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag {FGameplayTag()};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus {FGameplayTag()};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot {FGameplayTag()};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType {FGameplayTag()};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel {1};
};

inline bool operator==(const FSavedAbility& lhs, const FSavedAbility& rhs)
{
	return lhs.AbilityTag.MatchesTagExact(rhs.AbilityTag);
}

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

	/* Abilities */
	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FSavedAbility> SaveAbilities;
	/* End Abilities */

	UPROPERTY(BlueprintReadWrite, SaveGame)
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasSavedMap(const FString& InMapName);
};
