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
		AActor* PawnOwner = nullptr,
		float PitchOverride = 0.0f);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread{90.f};
};
