// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilitySystemComponentRegistrated, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

class UNiagaraSystem;
class UAnimMontage;

USTRUCT(BlueprintType, Blueprintable)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage{nullptr};;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* SoundImpact{nullptr};
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetPlayerLevel();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ECharacterClass GetCharacterClass();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingWrapping(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage(const FGameplayTag HitTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontage();

	virtual void Die() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetEffectByTag(const FGameplayTag&Tag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& Tag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMinionCount(int32 Amount);

	virtual FOnAbilitySystemComponentRegistrated GetOnAbilitySystemComponentRegistratedDelegate() = 0;
	virtual FOnDeath GetOnDeathDelegate() = 0;
};
