// Copyright - none


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

void UAuraDamageGameplayAbility::DoDamage(AActor* TargetActor)
{
	
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.0f);

	const float ScaledDamage = DamageScale.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParam UAuraDamageGameplayAbility::MakeDamageEffectParamsFromDefaultClass(AActor* TargetActor) const
{
	FDamageEffectParam DamageEffectParam;
	DamageEffectParam.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParam.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParam.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParam.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParam.BaseDamage = DamageScale.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParam.AbilityLevel = GetAbilityLevel();
	DamageEffectParam.DamageType = DamageType;
	DamageEffectParam.DebufChance = DebufChance;
	DamageEffectParam.DebuffDamage = DebuffDamage;
	DamageEffectParam.DebuffDuration = DebuffDuration;
	DamageEffectParam.DebuffFrequency = DebuffFrequency;
	DamageEffectParam.DeathImpulseMagnitude = DeathImpulseMagnitude;
	return DamageEffectParam;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedAttackMontage(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() != 0)
	{
		const int32 RandomIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[RandomIndex];
	}
	return FTaggedMontage();
}
