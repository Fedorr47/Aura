// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
public:

	/* Highlight interface*/
	virtual void HighlightActor_Implementation() override;
	/*End Highlight interface*/

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

//--------------------------------------------------------------------------------------------------------------------//


	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationWorld;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:

	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

//--------------------------------------------------------------------------------------------------------------------//

	
};
