// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToExperiencePoints(int32 InExperiencePoints);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetExperiencePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddPlayerLevel(int32 InPlayerLevels);

	UFUNCTION(BlueprintNativeEvent)
	void AddAttributePoints(int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddSpellPoints(int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForExperiencePoints(int32 InExperiencePoints) const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
