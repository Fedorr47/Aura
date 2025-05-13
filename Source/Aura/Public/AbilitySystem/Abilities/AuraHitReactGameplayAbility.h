// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraHitReactGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraHitReactGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitRaction")
	FGameplayTag HitReactType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> HitReactionEffectClass;
};
