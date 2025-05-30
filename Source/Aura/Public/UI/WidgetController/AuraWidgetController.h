// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAuraAttributeSet;
class AAuraPlayerState;
class AAuraPlayerController;
class UAttributeSet;
class UAbilitySystemComponent;
class UAbilityInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(
		APlayerController* PC,
		APlayerState* PS,
		UAbilitySystemComponent* ASC,
		UAttributeSet* AS
		) :
		PlayerController(PC),
		PlayerState(PS),
		AbilitySystemComponent(ASC),
		AttributeSet(AS)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent{nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet{nullptr};
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();
	void BroadcastAbilityInfo();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Mesages")
	FAbilityInfoSignature AbilityInfoDelegate;
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInformation;

	AAuraPlayerController *GetAuraPC();
	AAuraPlayerState* GetAuraPS();
	UAuraAbilitySystemComponent* GetAuraASC();
	UAuraAttributeSet* GetAuraAS();
};
