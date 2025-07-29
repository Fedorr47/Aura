// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "SpawnVolume.generated.h"

class AEnemySpawner;
class UBoxComponent;

UCLASS()
class AURA_API ASpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached {false};

protected:

	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawner*> SpawnPoints;
	
//--------------------------------------------------------------------------------------------------------------------//	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
