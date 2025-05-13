// Copyright - none


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Up = FVector::UpVector;

	const float UnitRadius = GetMaxMinionCapsuleRadius();
	const float MinArcLength = 2.0f * UnitRadius * 1.1f;

	const float OffsetMin = SpawnRange.GetLowerBoundValue();
	const float OffsetMax = SpawnRange.GetUpperBoundValue();
	const int32 TotalToSpawn = FMath::Max(NumMinions, 1);

	TArray<FVector> SpawnLocations;
	int32 Spawned = 0;
	float RingBaseRadius = UnitRadius * 2.0f;

	while (Spawned < TotalToSpawn)
	{
		const float RingRadius = RingBaseRadius;
		float AngleDegrees = 0.f;
		const float AngleStepDegrees = FMath::RadiansToDegrees(MinArcLength / RingRadius);

		while (AngleDegrees < 360.f && Spawned < TotalToSpawn)
		{
			const float AngleRad = FMath::DegreesToRadians(AngleDegrees);
			const FVector Dir = Forward.RotateAngleAxis(AngleDegrees, Up);
			const float RandOffset = FMath::FRandRange(OffsetMin, OffsetMax);
			float FinalRadius = RingRadius + RandOffset;

			FVector TryLoc = Origin + FVector(
			FinalRadius * FMath::Cos(AngleRad),
			FinalRadius * FMath::Sin(AngleRad),
			0.0f
			);
			
			if (IsSpawnPointValid(Origin, TryLoc))
			{
				FHitResult Hit;
				if (GetWorld()->LineTraceSingleByChannel(Hit, TryLoc + FVector(0,0,400), TryLoc - FVector(0,0,400), ECC_Visibility))
				{
					TryLoc = Hit.ImpactPoint;
				}

				SpawnLocations.Add(TryLoc);
				DrawDebugSphere(GetWorld(), TryLoc, 18.f, 12, FColor::Green, false, 3.0f, 0, 1.0f);
				++Spawned;
			}
			else
			{
				DrawDebugSphere(GetWorld(), TryLoc, 18.f, 12, FColor::Red, false, 3.0f, 0, 1.0f);
			}
			
			AngleDegrees += AngleStepDegrees;
		}
		
		RingBaseRadius += OffsetMax + UnitRadius * 2.0f;
	}

	return SpawnLocations;
}

float UAuraSummonAbility::GetMaxMinionCapsuleRadius() const
{
	float MaxRadius = 0.0f;

	for (const TSubclassOf<APawn>& MinionClass : MinionsClass)
	{
		if (!MinionClass) continue;

		const APawn* DefaultPawn = MinionClass->GetDefaultObject<APawn>();
		if (const ACharacter* CharacterCDO = Cast<ACharacter>(DefaultPawn))
		{
			if (const UCapsuleComponent* Capsule = CharacterCDO->GetCapsuleComponent())
			{
				MaxRadius = FMath::Max(MaxRadius, Capsule->GetScaledCapsuleRadius());
			}
		}
	}

	return MaxRadius;
}

bool UAuraSummonAbility::IsSpawnPointValid(const FVector& From, const FVector& To)
{
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetAvatarActorFromActorInfo());

	if (GetWorld()->LineTraceSingleByChannel(Hit, From, To, ECC_Visibility, Params))
	{
		return false;
	}

	TArray<FOverlapResult> Overlaps;
	const float Radius = GetMaxMinionCapsuleRadius();
	FCollisionShape Shape = FCollisionShape::MakeSphere(Radius);

	return !GetWorld()->OverlapMultiByChannel(Overlaps, To, FQuat::Identity, ECC_Pawn, Shape, Params);
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomSummoningClass() const
{
	if (MinionsClass.Num() == 0) return nullptr;
	const int32 RandomIndex = FMath::RandRange(0, MinionsClass.Num() - 1);
	return MinionsClass[RandomIndex];
}
