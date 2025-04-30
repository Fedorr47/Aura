// Copyright - none


#include "AbilitySystem/AuraAbilitySystemLibrary.h"


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	return GetWidgetController<UOverlayWidgetController>(WorldContextObject, &AAuraHUD::GetOverlayWidgetController);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	return GetWidgetController<UAttributeMenuWidgetController>(WorldContextObject, &AAuraHUD::GetAttributeWidgetController);
}