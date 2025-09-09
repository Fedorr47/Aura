// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CheatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCheatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICheatInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsUnlimitedMana();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetUnlimitedMana(bool NewValue);
};
