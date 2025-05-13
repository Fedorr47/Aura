// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	float GetMaxMinionCapsuleRadius() const;
	bool IsSpawnPointValid(const FVector& From, const FVector& To) const;

	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomSummoningClass() const;

	//----------------------------------------------------------//
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions {0};

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionsClass;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	FFloatRange SpawnRange;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread {90.0f};
};
