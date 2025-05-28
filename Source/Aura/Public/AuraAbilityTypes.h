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

	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }

	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	float GetDebuffDamage() const { return DebuffDamage; }

	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	float GetDebuffDuration() const { return DebuffDuration; }
	
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	float GetDebuffFrequency() const { return DebuffFrequency; }

	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	void SetDeathImpulse(FVector InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	FVector GetDeathImpulse() const { return DeathImpulse; }


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

	UPROPERTY()
	bool bIsSuccessfulDebuff{false};

	UPROPERTY()
	float DebuffDamage{0.0f};

	UPROPERTY()
	float DebuffDuration{0.0f};

	UPROPERTY()
	float DebuffFrequency{0.0f};

	UPROPERTY()
	FVector DeathImpulse{FVector::ZeroVector};

	UPROPERTY()
	float DeathImpulseMagnitude{0.0f};
	
	TSharedPtr<FGameplayTag> DamageType;
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

	UPROPERTY(EditDefaultsOnly)
	float DeathImpulseMagnitude{0.0f};
	UPROPERTY(EditDefaultsOnly)
	FVector DeathImpulse{FVector::ZeroVector};
};