// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
protected:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles( 
		const FVector& ProjectileTargetLocation,
		const FGameplayTag& SocketType,
		bool bOverridePitch = false,
		AActor* HomingTargetActor = nullptr,
		float PitchOverride = 0.0f);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread{90.0f};

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin{1600.0f};

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax{3200.0f};

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchingHomingProjectiles{true};
};
