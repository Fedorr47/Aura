// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"



/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void DoDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParam MakeDamageEffectParamsFromDefaultClass(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin =  FVector::ZeroVector) const;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedAttackMontage(const TArray<FTaggedMontage>& TaggedMontages) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat DamageScale;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebufChance {0.2f};
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage{5.0f};
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency{1.0f};
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration{5.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude {60.0f};
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackImpulseMagnitude {60.0f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HitRaction")
	FGameplayTag HitReactType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage{false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius{0.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius{0.0f};
};
