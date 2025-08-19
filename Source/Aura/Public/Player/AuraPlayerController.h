// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "Player/Inv_PlayerController.h"
#include "AuraPlayerController.generated.h"

class IHighlightInterface;
class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UAuraInputConfig;
class UInputMappingContext;
class UInputAction;
class UAuraAbilitySystemComponent;

struct FInputActionValue;

UENUM(BlueprintType)
enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingObject,
	NotTargeting
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public AInv_PlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumbers(float DamageValue, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle();
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
	UFUNCTION(BlueprintCallable)
	void SetMagicCircleMaterial(UMaterialInterface* InMaterial);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);
	
	void ShiftPressed() {bShiftPressed = true;}
	void ShiftReleased(){bShiftPressed = false;}

	void CursorTrace();
	void AutoRun();
	
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetAbilitySystemComponent();

	void UpdateMagicCircleLocation();

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	// -------------------------------------//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY()
	TObjectPtr<AActor> LastActor{nullptr};
	UPROPERTY()
	TObjectPtr<AActor> ThisActor{nullptr};
	FHitResult CursorHit;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	bool bShiftPressed = false;

	FVector CachedDestination{FVector::ZeroVector};
	float FollowTime{0.0f};
	float ShortPressThreshold{0.5f};
	bool bAutoRunning{false};
	ETargetingStatus TargetingStatus{ETargetingStatus::NotTargeting};

	UPROPERTY(EditDefaultsOnly, Category="Input")
	float AutoRunAcceptableRadius{50.0f};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> SplineComponent{nullptr};

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
};

