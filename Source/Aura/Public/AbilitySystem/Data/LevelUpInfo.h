// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement{0};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointsReward{1};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward{1};
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LevelUpInfo")
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	int32 FindLevelUpInfoByXP(int32 InXP) const;
};
