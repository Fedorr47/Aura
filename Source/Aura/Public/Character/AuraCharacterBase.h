// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

#define PlayerTag FName("Player")
#define EnemyTag FName("Enemy")

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;
struct FGameplayTag;
struct FDamageEvent;
class AController;

UENUM(Blueprintable, BlueprintType)
enum class ESkeletalSocketType : uint8
{
	WeaponSocket,
	MeshSocket
};

USTRUCT(Blueprintable, BlueprintType)
struct FSkeletalSocketType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkeletalSocketType SocketType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
};

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	virtual void InitAbilityActorInfo();
	
	virtual void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void ShockTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	virtual void BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UFUNCTION(BlueprintCallable)
	void SetHitReactMontages(TArray<FTaggedMontage> InHitMontages);
	UFUNCTION(BlueprintCallable)
	void SetAttackMontages(TArray<FTaggedMontage> InAttackMontages);
	
	/* Combat Interface */
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation(const FGameplayTag HitTag) override;
	virtual TArray<FTaggedMontage> GetAttackMontage_Implementation() override;
	virtual UNiagaraSystem* GetEffectByTag_Implementation(const FGameplayTag& Tag) override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& Tag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void SetMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual void Knockback(const FVector& KnockbackImpulse) override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual bool IsBeingInShock_Implementation() override;
	virtual void SetBeingInShock_Implementation(bool InShock) override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	virtual FOnAbilitySystemComponentRegistrated& GetOnAbilitySystemComponentRegistratedDelegate() override;
	virtual FOnDeath GetOnDeathDelegate();
	/* End Combat Interface */
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UFUNCTION(BlueprintCallable)
	bool IsHitReacting();

	USkeletalMeshComponent* GetWeapon() { return Weapon; }

	UFUNCTION()
	virtual void OnRep_Shocked();

	UFUNCTION()
	virtual void OnRep_Burned();

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;
	
	//-----------------------------------------------------------------------//
	FOnAbilitySystemComponentRegistrated OnAbilitySystemComponentRegistrated;
	FOnDeath OnDeath;
	FOnDamageSignature OnDamageDelegate;
	
	//-----------------------------------------------------------------------//
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting {false};

	UPROPERTY(ReplicatedUsing=OnRep_Shocked, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned{false};

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly, Category = "Combat")
	bool bIsBurned{false};

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bBeingInShock{false};

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float MaxWalkSpeed {0.0f};

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TMap<FGameplayTag, FSkeletalSocketType> TagToWeaponTipSocketInfo;

	
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
	/* Dissolve Effects end */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Class Defaults")
	ECharacterClass CharacterClass{ECharacterClass::Player};

	UPROPERTY(Transient)
	bool bDead {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	TMap<FGameplayTag, UNiagaraSystem*> EffectsToTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character|Effects")
	USoundBase* DeathSound {nullptr};

	UPROPERTY()
	int32 MinionCount {0};

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> PassiveProtectionEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> PassiveLifeSiphonEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> PassiveManaSiphonEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
	
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

	USceneComponent* GetSkeletalSocketOwner(ESkeletalSocketType InSkeletalSocketType);

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> HitMontages;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;
};
