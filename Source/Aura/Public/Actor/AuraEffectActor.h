// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UWidgetComponent;
class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
	None
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
	None
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();

	UFUNCTION(BlueprintCallable)
	void StartRotation();

	virtual void ShowActorWidget(bool bVisibility, AActor* TargetActor);
	
//--------------------------------------------------------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> InstantiateGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy InstantiateEffectApplicationPolicy {EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy DurationEffectApplicationPolicy{EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy{EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effect")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy{EEffectRemovalPolicy::DoNotRemove};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effect")
	float ActorLevel{1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bRotates {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float RotationRate {45.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bSinusoidalMovement{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineAmplitude {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineConstant {1.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	FVector InitialLocation {0.0f, 0.0f, 0.0f};

	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation {0.0f, 0.0f, 0.0f};

	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation {0.0f, 0.0f, 0.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Widget")
	TObjectPtr<UWidgetComponent> ActorWidget;
	
private:
	void ItemMovement(float DeltaTime);
//--------------------------------------------------------------------------------------------------------------------//
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	TArray<FActiveGameplayEffectHandle> ActiveInstantEffectHandles;
	TArray<FActiveGameplayEffectHandle> ActiveDurationEffectHandles;

	float RunningTime { 0.0f };
};
