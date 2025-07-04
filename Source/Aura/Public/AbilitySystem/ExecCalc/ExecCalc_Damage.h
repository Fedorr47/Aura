// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectSpec Spec,
	                     FAggregatorEvaluateParameters EvaluateParams,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const;
	float DetermineDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectSpec Spec,
	                     FAggregatorEvaluateParameters EvaluateParams,
	                     AActor* SourceAvatar,
	                     AActor* TargetAvatar,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const;

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
