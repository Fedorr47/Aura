// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

namespace MMC_MaxHealthConstants
{
	inline constexpr float BaseValue = 80.0f;
	inline constexpr float Magnifier = 2.5f;
	inline constexpr float AdditionalVal = 10.0f;
}

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:	
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
