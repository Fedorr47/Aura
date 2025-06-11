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

FDamageEffectParam UAuraDamageGameplayAbility::MakeDamageEffectParamsFromDefaultClass(
		AActor* TargetActor /*= nullptr*/,
		FVector InRadialDamageOrigin /*= FVector::ZeroVector*/,
		bool bOverrideKnockbackDirection /*= false*/,
		FVector KnockbackDirectionOverride /*= FVector::ZeroVector*/,
		bool bOverrideDeathImpulse /*=false*/,
		FVector DeathImpulseDirectionOverride /*= FVector::ZeroVector*/,
		bool bOverridePitch /*= false*/,
		float PitchOverride /*= 0.0f*/) const
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
	DamageEffectParam.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;

	if (IsValid(TargetActor))
	{
		FRotator TargetRotator = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			TargetRotator.Pitch = PitchOverride;
		}
		const FVector TargetLocation = TargetRotator.Vector();
		if (!bOverrideKnockbackDirection)
		{
			DamageEffectParam.KnockbackImpulse = TargetLocation * KnockbackImpulseMagnitude;
		}
		if (!bOverrideDeathImpulse)
		{
			DamageEffectParam.DeathImpulse = TargetLocation * DeathImpulseMagnitude;
		}
	}
	
	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		DamageEffectParam.KnockbackImpulse = KnockbackDirectionOverride * KnockbackImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParam.KnockbackImpulse = KnockbackRotation.Vector() * KnockbackImpulseMagnitude;
		}
	}

	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		DamageEffectParam.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathRotation = DeathImpulseDirectionOverride.Rotation();
			DeathRotation.Pitch = PitchOverride;
			DamageEffectParam.DeathImpulse = DeathRotation.Vector() * DeathImpulseMagnitude;
		}
	}

	if (bIsRadialDamage)
	{
		DamageEffectParam.bIsRadialDamage = bIsRadialDamage;
		DamageEffectParam.RadialDamageOrigin = InRadialDamageOrigin;
		DamageEffectParam.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParam.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	
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

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return DamageScale.GetValueAtLevel(GetAbilityLevel());
}
