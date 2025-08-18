// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "PickUpAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UPickUpAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "PickUpAbility")
	void EquipItem(AActor* Item);
};
