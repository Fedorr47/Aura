// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	virtual void BeginPlay() override;

	FORCEINLINE UCameraComponent* GetTopDownCameraComponent() const { return CameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* End Combat Interface */

	/* Player Interface */
	virtual void AddToExperiencePoints_Implementation(int32 InExperiencePoints) override;
	virtual int32 GetExperiencePoints_Implementation() const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 FindLevelForExperiencePoints_Implementation(int32 InExperiencePoints) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddPlayerLevel_Implementation(int32 InPlayerLevels) override;
	virtual void AddAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddSpellPoints_Implementation(int32 SpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const;
	virtual int32 GetSpellPoints_Implementation() const;
	/* End Player Interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpEffect;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpEffect();
};
