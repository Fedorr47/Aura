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
	const float ArcSpacing = 2.0f * UnitRadius * 1.1f;

	const float OffsetMin = SpawnRange.GetLowerBoundValue();
	const float OffsetMax = SpawnRange.GetUpperBoundValue();

	const int32 TotalToSpawn = FMath::Max(NumMinions, 1);

	TArray<FVector> SpawnLocations;
	int32 Spawned = 0;
	float RingBaseRadius = UnitRadius * 2.0f;

	while (Spawned < TotalToSpawn)
	{
		float RingRadius = RingBaseRadius;
		float Circumference = 2 * PI * (RingRadius + OffsetMax);
		int32 PointsOnRing = FMath::FloorToInt(Circumference / ArcSpacing);
		PointsOnRing = FMath::Max(PointsOnRing, 1);

		for (int32 i = 0; i < PointsOnRing && Spawned < TotalToSpawn; ++i)
		{
			float Angle = (2 * PI / PointsOnRing) * i;
			FVector Direction = Forward.RotateAngleAxis(FMath::RadiansToDegrees(Angle), Up);
			float RandomOffset = FMath::FRandRange(OffsetMin, OffsetMax);
			FVector SpawnLoc = Origin + Direction * (RingRadius + RandomOffset);

			// Line trace to ground
			FHitResult Hit;
			const FVector TraceStart = SpawnLoc + FVector(0.f, 0.f, 400.f);
			const FVector TraceEnd = SpawnLoc - FVector(0.f, 0.f, 400.f);
			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
			{
				SpawnLoc = Hit.ImpactPoint;
			}

			SpawnLocations.Add(SpawnLoc);
			//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), SpawnLoc, 16.0f, 12, FColor::Green, 3.0f, 1.0f);
			++Spawned;
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

TSubclassOf<APawn> UAuraSummonAbility::GetRandomSummoningClass() const
{
	if (MinionsClass.Num() == 0) return nullptr;
	const int32 RandomIndex = FMath::RandRange(0, MinionsClass.Num() - 1);
	return MinionsClass[RandomIndex];
}
