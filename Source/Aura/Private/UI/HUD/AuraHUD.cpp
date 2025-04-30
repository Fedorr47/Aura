// Copyright - none


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/Widgets/AuraUserWidget.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = InitWidgetControllerInternal<UOverlayWidgetController>(
		this, Params, OverlayWidgetControllerClass);
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeWidgetController == nullptr)
	{
		AttributeWidgetController = InitWidgetControllerInternal<UAttributeMenuWidgetController>(
		this, Params, AttributeWidgetControllerClass);
	}
	return AttributeWidgetController;
}

void AAuraHUD::InitHUD(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	InitOverlay(WidgetControllerParams);
	AttributeWidgetController = InitWidgetControllerInternal<UAttributeMenuWidgetController>(
		this, WidgetControllerParams, AttributeWidgetControllerClass);
}

void AAuraHUD::InitOverlay(const FWidgetControllerParams& Params)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill out BP_AuraHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	
	OverlayWidgetController = InitWidgetControllerInternal<UOverlayWidgetController>(
		this, Params, OverlayWidgetControllerClass);
	
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}
