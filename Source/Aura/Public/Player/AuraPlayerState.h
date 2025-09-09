// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/CheatInterface.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FStatChangedSignature, int32 /*StatValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelPlayerChangedSignature, int32 /*StatValue*/, bool /*bLevelUp*/);
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
	,public ICheatInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	FORCEINLINE int32 GetPlayerLevel() const {return Level;}
	void SetLevel(const int32 NewAmount);
	void AddToLevel(const int32 NewAmount);
	
	FORCEINLINE int32 GetExperiencePoints() const {return ExperiencePoints;}
	FORCEINLINE int32 GetAttributePoints() const {return AttributePoints;}
	FORCEINLINE int32 GetSpellPoints() const {return SpellPoints;}
	
	void SetExperiencePoints(const int32 NewAmount);
	void AddExperiencePoints(const int32 NewAmount);
	void AddAttributePoints(const int32 NewAmount);
	void AddSpellPoints(const int32 NewAmount);
	void SetAttributePoints(const int32 NewAmount);
	void SetSpellPoints(const int32 NewAmount);
	
	FStatChangedSignature OnExperiencePointsChangedDelegate;
	FOnLevelPlayerChangedSignature OnLevelChangedDelegate;
	FStatChangedSignature OnAttributesPointsChangedDelegate;
	FStatChangedSignature OnSpellPointsChangedDelegate;

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

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints{0};

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints{0};

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_ExperiencePoints(int32 OldExperiencePoints);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

public:
	UFUNCTION(Server, Reliable)
	void Server_SetUnlimitedMana(bool NewValue);
#if !UE_BUILD_SHIPPING
public:
	/* Cheat interface */
	virtual bool IsUnlimitedMana_Implementation() override;
	virtual void SetUnlimitedMana_Implementation(bool NewValue) override;
	/* End Cheat interface*/
	
	//------------------------------------------------------------------------//

private:
	bool IsUnlimitedMana{false};
#endif
	
};
