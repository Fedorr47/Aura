// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);

	template <typename TController>
	static TController* GetWidgetController(const UObject* WorldContextObject, TController* (AAuraHUD::*Getter)(const FWidgetControllerParams&))
	{
		if (!IsValid(WorldContextObject)) return nullptr;

		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				if (AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
				{
					UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
					UAttributeSet* AS = PS->GetAttributeSet();
					const FWidgetControllerParams Params(PC, PS, ASC, AS);
					return (AuraHUD->*Getter)(Params);
				}
			}
		}
		return nullptr;
	}
};
