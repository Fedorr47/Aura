// Copyright - none


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(AirResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(WaterResistance)

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, AirResistance,		Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance,	Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance,	Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, WaterResistance,		Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().AirResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DeternineDebuff(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec Spec,
	FAggregatorEvaluateParameters EvaluateParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	for (auto& PairDebuff : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag DamageType = PairDebuff.Key;
		const FGameplayTag DebuffType = PairDebuff.Value;
		const float DebuffDamageValue = Spec.GetSetByCallerMagnitude(DamageType, false, -1.0f);
		if (DebuffDamageValue > -1.0f)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, 0.0f);

			float TargetDebuffResistance = 0.0f;
			const FGameplayTag DebuffResistanceType = GameplayTags.DamageTypesToResistance[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
				InTagsToCaptureDefs[DebuffResistanceType], EvaluateParams, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max(TargetDebuffResistance, 0.0f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.0f;
			const bool bIsDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bIsDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				const float DebuffDamage	= Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage,		false, -1.0f);
				const float DebuffDuration	= Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration,	false, -1.0f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency,	false, -1.0f);

				UAuraAbilitySystemLibrary::SetSuccessfulDebuff(ContextHandle, bIsDebuff);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);
				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

float UExecCalc_Damage::DetermineDamage(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec Spec,
	FAggregatorEvaluateParameters EvaluateParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	float Damage = 0.0f;
	for (auto& PairDamage : GameplayTags.DamageTypesToResistance)
	{
		const FGameplayTag ResistanceTag = PairDamage.Value;
		checkf(InTagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag [%s] in ExecCalDamage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CapturedDef = InTagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(PairDamage.Key, false);
		
		if (DamageTypeValue > 0.0f)
		{
			float ResistanceVal = 0.0f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturedDef, EvaluateParams, ResistanceVal);
			ResistanceVal = FMath::Clamp(ResistanceVal, 0.0f, 100.0f);
			DamageTypeValue *= (100.0f - ResistanceVal) / 100.0f;
		}
		
		Damage += DamageTypeValue;
	}
	return Damage;
}

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor,					DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance,				DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance,		DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage,		DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance,	DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration,		DamageStatics().ArmorPenetrationDef);

	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire,		DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Air,		DamageStatics().AirResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane,		DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning,	DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical,	DamageStatics().PhysicalResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Water,		DamageStatics().WaterResistanceDef);
	
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}	

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FGameplayEffectContextHandle Context = Spec.GetContext();

	auto GetClampedCapturedMagnitude = [&](const FGameplayEffectAttributeCaptureDefinition& Def) -> float
	{
		float Value = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Def, EvaluateParams, Value);
		return FMath::Max(0.0f, Value);
	};

	auto EvalCurve = [&](const FString& CurveName, int32 Level, float Default = 1.0f) -> float
	{
		if (const FRealCurve* Curve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName(*CurveName), FString()))
		{
			return Curve->Eval(Level);
		}
		return Default;
	};

	// Debuff
	DeternineDebuff(ExecutionParams, Spec, EvaluateParams, TagsToCaptureDefs);

	// Get Damage Set by Caller Magnitude
	float Damage = DetermineDamage(ExecutionParams, Spec, EvaluateParams, TagsToCaptureDefs);

	// Critical hit chance
	float SourceCriticalHitChance = GetClampedCapturedMagnitude(DamageStatics().CriticalHitChanceDef);

	// Critical hit resistance
	float TargetCriticalHitResistanceChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistanceChance);
	TargetCriticalHitResistanceChance = FMath::Max(0.0f, TargetCriticalHitResistanceChance);

	float TargetCriticalHitResistanceCoefficient = EvalCurve("CriticalHitResistance", TargetPlayerLevel);
	TargetCriticalHitResistanceChance *= TargetCriticalHitResistanceCoefficient;
	
	// Critical hit damage
	float SourceCriticalHitDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParams, SourceCriticalHitDamage);
	SourceCriticalHitChance =  FMath::Max(0.0f, SourceCriticalHitChance - TargetCriticalHitResistanceChance);
	
	
	// Capture BlockChance on a Target
	float TargetBlockChance = GetClampedCapturedMagnitude(DamageStatics().BlockChanceDef);
	
	// TODO: change 100.0f to MaxBlockChance
	const bool IsSuccessfulBlock = FMath::RandRange(0.0f, 100.0f) < TargetBlockChance;
	Damage = IsSuccessfulBlock ? (Damage / 2.0f) : Damage;
	
	UAuraAbilitySystemLibrary::SetBlockedHit(Context, IsSuccessfulBlock);
	
	// Armor
	float TargetArmor = GetClampedCapturedMagnitude(DamageStatics().ArmorDef);
	
	// Armor Penetration
	float SourceArmorPenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.0f, SourceArmorPenetration);
	
	float ArmorPenetrationCoefficient = EvalCurve("ArmorPenetration", SourcePlayerLevel);
	float EffectiveArmorCoefficient = EvalCurve("EffectiveArmor", TargetPlayerLevel);
	
	// TODO: change 100.0f to MaxArmorPenetration
	const float EffectiveArmor = TargetArmor *= FMath::Max((100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient), 0.0f) / 100.0f;
	Damage *= (100.0f  - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

	const bool IsSuccessfulCriticalHit = FMath::RandRange(0.0f, 100.0f) < SourceCriticalHitDamage;
	Damage = IsSuccessfulCriticalHit ? ((Damage * 2.0f) + SourceCriticalHitDamage) : Damage;

	UAuraAbilitySystemLibrary::SetCriticalHit(Context, IsSuccessfulCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
