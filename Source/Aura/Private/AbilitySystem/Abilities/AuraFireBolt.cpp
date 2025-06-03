// Copyright - none


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
  		"<Default>Launches %i bolts of fire, exploding on impact and dealing</>"
  		"<Damage> %d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfProjectiles),
					   ScaledDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Launches %i bolts of fire, exploding on impact and dealing</>"
		  "<Damage> %d </>"
		"<Default>fire damage with a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfProjectiles),
					   ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(
	const FVector& ProjectileTargetLocation,
	const FGameplayTag& SocketType,
	bool bOverridePitch,
	AActor* PawnOwner,
	float PitchOverride /*= 0.0f*/)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketType);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();
	
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpaceRotators(Forward, FVector::UpVector, ProjectileSpread, NumOfProjectiles);
	for (const FRotator& Rotator : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	
		Projectile->DamageEffectParam = MakeDamageEffectParamsFromDefaultClass();
	
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
