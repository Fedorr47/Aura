// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

#define PlayerTag FName("Player")
#define EnemyTag FName("Enemy")

class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;
struct FGameplayTag;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	virtual void InitAbilityActorInfo();
	
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	UFUNCTION(BlueprintCallable)
	void SetHitReactMontages(TMap<FGameplayTag, UAnimMontage*> InHitMontages);
	virtual UAnimMontage* GetHitReactMontage_Implementation(const FGameplayTag HitTag) override;
	
	virtual void Die() override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	//-----------------------------------------------------------------------//
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting {false};

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float MaxWalkSpeed {0.0f};
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

	virtual FVector GetCombatSocketLocation() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/* Dissolve Effects*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	//-----------------------------------------------------------------------------//
	virtual void InitializeDefaultAttributes() const;
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	void AddCharacterAbilities();

	/* Dissolve Effects*/
	void Dissolve();
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
private:

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> HitMontages;
};
