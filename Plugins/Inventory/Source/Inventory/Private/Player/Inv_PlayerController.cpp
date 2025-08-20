#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inv_DisplayItemsComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Default;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		for (UInputMappingContext* CurrentContext : DefaultIMCs)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	//CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
}

void AInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//TraceForItem();
	TraceCursorForItem();
}

void AInv_PlayerController::SetInventoryHUDWidget(UInv_HUDWidget* InventoryHUDWidget)
{
	HUDWidget = InventoryHUDWidget;
}

void AInv_PlayerController::PrimaryInteract()
{
}

void AInv_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController())
	{
		return;
	}

	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AInv_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))
	{
		return;
	}
	FVector2D ViewportSize = FVector2D::ZeroVector;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.0f;

	FVector TraceStart;
	FVector Forward;
	if (!UGameplayStatics::DeprojectScreenToWorld(
		this,
		ViewportCenter,
		TraceStart,
		Forward))
	{
		return;
	}
	
	FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (ThisActor == LastActor)
	{
		return;
	}

	if (ThisActor.IsValid())
	{
		
	}

	if (LastActor.IsValid())
	{
		
	}
}

void AInv_PlayerController::TraceCursorForItem()
{
	if (!IsValid(GEngine))
	{
		return;
	}
	
	FHitResult CursorHit;
	GetHitResultUnderCursor(ItemTraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		LastActor = nullptr;
		ThisActor = nullptr;
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
		return;
	}
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
	}
	
	if (ThisActor == LastActor)
	{
		return;
	}

	if (ThisActor.IsValid())
	{
		UInv_DisplayItemsComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_DisplayItemsComponent>();
		if (!IsValid(ItemComponent))
		{
			return;
		}
		if (IsValid(HUDWidget))
		{
			HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
		}
	}

	if (ThisActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Started tracing a new actor"));
	}

	if (LastActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Stopped tracing a new actor"));
	}
}
