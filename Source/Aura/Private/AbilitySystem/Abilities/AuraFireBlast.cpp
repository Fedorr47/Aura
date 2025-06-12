// Copyright - none


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"
#include "Character/AuraCharacter.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		"<Title>FIRE BLAST:</>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
		  "<Default>Launches %d </>"
		  "<Default>fire balls in all directions, each coming back and </>"
		  "<Default>exploding upon return, causing </>"
		  "<Damage> %.1f </>"
		  "<Default> radial fire damage with </>"
		"<Default>a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfFireballs),
					   ScaledDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const float ScaledDamage = DamageScale.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
	"<Title>NEXT LEVEL:</>\n"
	"<Small>Level: </><Level>%d</>\n"
	"<Small>ManaCost: </><ManaCost>%.1f</>\n"
	"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
	  "<Default>Launches %d </>"
	  "<Default>fire balls in all directions, each coming back and </>"
	  "<Default>exploding upon return, causing </>"
	  "<Damage> %.1f </>"
	  "<Default> radial fire damage with </>"
	"<Default>a chance to burn</>\n\n"),
					   Level,
					   ManaCost,
					   Cooldown,
					   FMath::Min(Level, NumOfFireballs),
					   ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpaceRotators(
		Forward,
		FVector::UpVector,
		360.0f,
		NumOfFireballs);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParam = MakeDamageEffectParamsFromDefaultClass();
		FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromDefaultClass();
		
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	
	return FireBalls;
}