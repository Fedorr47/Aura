// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FName PlayerStartTag{FName()};

	UPROPERTY()
	FString LoadSlotName{FString()};

	UPROPERTY()
	int32 LoadSlotIndex{0};
};
