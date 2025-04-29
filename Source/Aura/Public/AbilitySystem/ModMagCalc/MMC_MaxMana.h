// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

namespace MMC_MaxManaConstants
{
	inline constexpr float BaseValue = 90.0f;
	inline constexpr float Magnifier = 3.0f;
	inline constexpr float AdditionalVal = 15.0f;
}

/**
 * 
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
