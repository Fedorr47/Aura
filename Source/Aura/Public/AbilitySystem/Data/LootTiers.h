// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float ChanceToSpawn{0.0f};

	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxNumberToSpawn{0};

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "LootTiers|Spawning")
	bool bLoottLevelOverride{true};
};

UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "LootTiers|Spawning")
	TArray<FLootItem> GetLootItems();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;
};
