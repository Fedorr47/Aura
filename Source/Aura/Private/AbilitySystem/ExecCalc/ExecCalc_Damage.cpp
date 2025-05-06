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

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

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

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();
		
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);

		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Air, AirResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Water, WaterResistanceDef);
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

void UExecCalc_Damage::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

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

	// Get Damage Set by Caller Magnitude
	float Damage = 0.0f;
	for (auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag [%s] in ExecCalDamage"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CapturedDef = AuraDamageStatics().TagsToCaptureDefs [ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		if (DamageTypeValue > 0.0f)
		{
			float ResistanceVal = 0.0f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturedDef, EvaluateParams, ResistanceVal);
			ResistanceVal = FMath::Clamp(ResistanceVal, 0.0f, 100.0f);
			DamageTypeValue *= (100.0f - ResistanceVal) / 100.0f;
		}
		
		Damage += DamageTypeValue;
	}

	// Critical hit chance
	float SourceCriticalHitChance = GetClampedCapturedMagnitude(DamageStatics().CriticalHitChanceDef);

	// Critical hit resistance
	float TargetCriticalHitResistanceChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistanceChance);
	TargetCriticalHitResistanceChance = FMath::Max(0.0f, TargetCriticalHitResistanceChance);

	float TargetCriticalHitResistanceCoefficient = EvalCurve("CriticalHitResistance", TargetCombatInterface->GetPlayerLevel());
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
	
	float ArmorPenetrationCoefficient = EvalCurve("ArmorPenetration", SourceCombatInterface->GetPlayerLevel());
	float EffectiveArmorCoefficient = EvalCurve("EffectiveArmor", TargetCombatInterface->GetPlayerLevel());
	
	// TODO: change 100.0f to MaxArmorPenetration
	const float EffectiveArmor = TargetArmor *= FMath::Max((100.0f - SourceArmorPenetration * ArmorPenetrationCoefficient), 0.0f) / 100.0f;
	Damage *= (100.0f  - EffectiveArmor * EffectiveArmorCoefficient) / 100.0f;

	const bool IsSuccessfulCriticalHit = FMath::RandRange(0.0f, 100.0f) < SourceCriticalHitDamage;
	Damage = IsSuccessfulCriticalHit ? ((Damage * 2.0f) + SourceCriticalHitDamage) : Damage;

	UAuraAbilitySystemLibrary::SetCriticalHit(Context, IsSuccessfulCriticalHit);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
