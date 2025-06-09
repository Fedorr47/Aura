// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class AURA_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:
	APointCollection();
	
	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(
		const FVector& GroundLocation,
		int32 NumPoints,
		float YawOverride = 0.0f) const;

	//-------------------------------------------------------------------------//
	

protected:
	virtual void BeginPlay() override;
	
	//--------------------------------------------------------------------------//
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> ImmutablePointers;
	
private:

	int32 NumOfPointers{10};
};
