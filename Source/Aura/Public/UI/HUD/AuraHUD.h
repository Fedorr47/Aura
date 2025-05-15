// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAuraWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
class UAuraUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& Params);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& Params);
	
	void InitHUD(APlayerController* PC,
	             APlayerState* PS,
	             UAbilitySystemComponent* ASC,
	             UAttributeSet* AS);
	
private:
	void InitOverlay(const FWidgetControllerParams& Params);

	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeWidgetController;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	// Templates
	template <typename T>
	T* InitWidgetControllerInternal(
		UObject* Outer,
		const FWidgetControllerParams& Params,
		const TSubclassOf<T>& WidgetControllerClass)
	{
		T* WidgetController = NewObject<T>(Outer, WidgetControllerClass);
		WidgetController->SetWidgetControllerParams(Params);
		WidgetController->BindCallbacksToDependencies();
		return WidgetController;
	}
};
