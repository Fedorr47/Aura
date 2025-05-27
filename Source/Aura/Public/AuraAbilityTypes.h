#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAuraGameplayEffectContext::StaticStruct();
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }


	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

protected:

	UPROPERTY()
	bool bIsBlockedHit{false};

	UPROPERTY()
	bool bIsCriticalHit{false};
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

USTRUCT(BlueprintType)
struct FDamageEffectParam
{
	GENERATED_BODY()

	FDamageEffectParam(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject{nullptr};

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass{nullptr};

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent{nullptr};

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent{nullptr};

	UPROPERTY()
	float BaseDamage{0.0f};
	UPROPERTY()
	float AbilityLevel{1.0f};
	UPROPERTY()
	FGameplayTag DamageType{FGameplayTag()};
	
	UPROPERTY(EditDefaultsOnly)
	float DebufChance {0.2f};
	UPROPERTY(EditDefaultsOnly)
	float DebuffDamage{5.0f};
	UPROPERTY(EditDefaultsOnly)
	float DebuffFrequency{1.0f};
	UPROPERTY(EditDefaultsOnly)
	float DebuffDuration{5.0f};
};