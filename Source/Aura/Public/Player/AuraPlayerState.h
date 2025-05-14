// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FStatChangedSignature, int32 /*StatValue*/);
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	// Level
	FORCEINLINE int32 GetPlayerLevel() const {return Level;}
	void SetLevel(const int32 NewAmount);
	void AddToLevel(const int32 NewAmount);

	// ExperiencePoints
	FORCEINLINE int32 GetExperiencePoints() const {return ExperiencePoints;}
	void SetExperiencePoints(const int32 NewAmount);
	void AddExperiencePoints(const int32 NewAmount);
	
	FStatChangedSignature OnExperiencePointsChangedDelegate;
	FStatChangedSignature OnLevelChangedDelegate;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level{1};
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_ExperiencePoints)
	int32 ExperiencePoints{1};

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_ExperiencePoints(int32 OldExperiencePoints);
};
