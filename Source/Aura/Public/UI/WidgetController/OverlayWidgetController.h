// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class ULevelUpInfo;
struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAbilityInfo;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag{FGameplayTag()};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message {FText()};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image{nullptr};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	void OnExperiencePointsChanged(int32 ExperiencePoints);
	void OnLevelChanged(int32 LevelCount) const;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributies")
	FOnAttributeChangedSignature OnHeathChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributies")
	FOnAttributeChangedSignature OnMaxHeathChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributies")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributies")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributies")
	FMessageWidgetRowSignature 	MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangedSignature OnExperiencePointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnStatChangedSignature OnLevelPointsChangedDelegate;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<ULevelUpInfo> LevelUpInformation;
	
	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,
	const FGameplayTag& SlotTag,
	const FGameplayTag& PreviousSlotTag) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
