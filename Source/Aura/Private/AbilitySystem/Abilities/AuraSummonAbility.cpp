// Copyright - none


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Origin = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector UpVector = FVector::UpVector;

	const float CapsuleRadius = GetMaxMinionCapsuleRadius();
	const int32 Num = FMath::Max(NumMinions, 1);
	const float AngleStep = 2 * PI / Num;
	const float Radius = CapsuleRadius / FMath::Sin(PI / Num);

	TArray<FVector> SpawnLocations;

	for (int32 i = 0; i < Num; ++i)
	{
		const float AngleRad = i * AngleStep;
		const FVector Direction = ForwardVector.RotateAngleAxis(FMath::RadiansToDegrees(AngleRad), UpVector);
		FVector SpawnLocation = Origin + Direction * (Radius + FMath::FRandRange(
			SpawnRange.GetLowerBoundValue(), SpawnRange.GetUpperBoundValue()));
		FHitResult Hit;
		const FVector LineTraceStart = FVector(0.0f, 0.0f, 400.0f);
		const FVector LineTraceEnd = FVector(0.0f, 0.0f, -400.0f);
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			SpawnLocation + LineTraceStart,
			SpawnLocation - LineTraceEnd,
			ECollisionChannel::ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			SpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(SpawnLocation);

		// Debug - delete later
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), SpawnLocation, 18.0f, 12, FColor::Green, 3.0f, 1.0f);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), Origin, SpawnLocation, 4.0f, FColor::Red, 3.0f);
		// Debug End - delete later
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