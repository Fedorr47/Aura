// Copyright - none


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	for (int32 i = 0; i < NumOfPointers; ++i)
	{
		FString NewPointName = "Point" + FString::FromInt(i);
		ImmutablePointers.Emplace(CreateDefaultSubobject<USceneComponent>(*NewPointName));
		if (i == 0)
		{
			SetRootComponent(ImmutablePointers[i]);
		}
		else
		{
			ImmutablePointers[i]->SetupAttachment(GetRootComponent());
		}
	}
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,
	float YawOverride) const
{
	int32 NumOfImmutables = ImmutablePointers.Num();
	checkf(NumOfImmutables >= NumPoints, TEXT("Attempted to access ImmutablePointers out of bounds."));

	TArray<USceneComponent*> CopyOfArray;
	TObjectPtr<USceneComponent> ZeroPoint =  ImmutablePointers[0];
	ZeroPoint->SetWorldLocation(GroundLocation);
	for (int32 i = 0; i < NumPoints; ++i)
	{
		TObjectPtr<USceneComponent> Point = ImmutablePointers[i];
		if (i != 0)
		{
			FVector ToPoint = Point->GetComponentLocation() - ZeroPoint->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(ZeroPoint->GetComponentLocation() + ToPoint);
		}
		const FVector RaisedLocation = FVector(
			Point->GetComponentLocation().X,
			Point->GetComponentLocation().Y,
			Point->GetComponentLocation().Z + 500.0f);
		const FVector LowerLocation = FVector(
			Point->GetComponentLocation().X,
			Point->GetComponentLocation().Y,
			Point->GetComponentLocation().Z - 500.0f);

		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(
			this,
			IgnoreActors,
			TArray<AActor*>(),
			1500.0f,
			GetActorLocation());

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(
			HitResult,
			RaisedLocation,
			LowerLocation,
			FName(TEXT("BlockAll")), QueryParams);

		const FVector AdjustedLocation = FVector(
			Point->GetComponentLocation().X,
			Point->GetComponentLocation().Y,
			HitResult.ImpactPoint.Z);
		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		CopyOfArray.Add(Point);
	}

	return CopyOfArray;
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}
