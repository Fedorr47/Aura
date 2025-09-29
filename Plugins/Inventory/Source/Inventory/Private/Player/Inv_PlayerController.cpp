#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/Inv_Highlightable.h"
#include "Items/Components/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InventoryManager/Components/Inv_InventoryComponent.h"
#include "Widgets/HUD/Inv_HUDWidget.h"


AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInv_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid())
	{
		return;
	}
	InventoryComponent->ToggleInventoryMenu();
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

	InventoryComponent = FindComponentByClass<UInv_InventoryComponent>();

	//CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AInv_PlayerController::ToggleInventory);
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
	if (!ThisActor.IsValid())
	{
		return;
	}

	UInv_ItemComponent* ItemComponent  = ThisActor->FindComponentByClass<UInv_ItemComponent>();
	if (!IsValid(ItemComponent) || !InventoryComponent.IsValid())
	{
		return;
	}
	InventoryComponent->TryAddItem(ItemComponent);
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

bool AInv_PlayerController::IsWithinCursorXYRadius(APlayerController* PC, const AActor* Actor, float MaxXYRadiusUU)
{
	if (!PC || !Actor) return false;

	float MouseX, MouseY;
	if (!PC->GetMousePosition(MouseX, MouseY)) return false;

	FVector RayOrigin, RayDir;
	if (!PC->DeprojectScreenPositionToWorld(MouseX, MouseY, RayOrigin, RayDir)) return false;
	
	const FVector ALoc = Actor->GetActorLocation();
	const FPlane Plane(ALoc, FVector::UpVector);

	const FVector LineEnd = RayOrigin + RayDir * 100000.f;
	const FVector CursorOnPlane = FMath::RayPlaneIntersection(RayOrigin, LineEnd, Plane);

	if (!CursorOnPlane.IsNearlyZero() && CursorOnPlane.ContainsNaN() == false)
	{
		const float d2D = FVector::Dist2D(CursorOnPlane, ALoc);
		return d2D <= MaxXYRadiusUU;
	}
	return false;
}


bool AInv_PlayerController::ActorNotInRadius()
{
	if (!IsWithinCursorXYRadius(this, ThisActor.Get(), MaxCursorRadius))
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
		
		if (LastActor.IsValid() && LastActor != ThisActor)
			if (auto* Cmp = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
				IInv_Highlightable::Execute_UnHighlight(Cmp);

		ThisActor = nullptr;
		return true;
	}
	return false;
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
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
		}
		if (LastActor.IsValid())
		{
			if (UActorComponent* HighlightableActorComponent = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass());
				IsValid(HighlightableActorComponent))
			{
				IInv_Highlightable::Execute_UnHighlight(HighlightableActorComponent);
			}
		}
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(HUDWidget))
		{
			HUDWidget->HidePickupMessage();
			if (LastActor.IsValid())
				if (auto* ActorComponent = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass()))
					IInv_Highlightable::Execute_UnHighlight(ActorComponent);
		}
	}
	
	if (ThisActor == LastActor)
	{
		if (ActorNotInRadius()) return;
		return;
	}
	
	if (ThisActor.IsValid())
	{
		if (ActorNotInRadius()) return;
		
		if (UActorComponent* HighlightableActorComponent = ThisActor->FindComponentByInterface(UInv_Highlightable::StaticClass());
			IsValid(HighlightableActorComponent))
		{
			IInv_Highlightable::Execute_Highlight(HighlightableActorComponent);
		}
		
		UInv_ItemComponent* ItemComponent = ThisActor->FindComponentByClass<UInv_ItemComponent>();
		if (!IsValid(ItemComponent))
		{
			return;
		}
		if (IsValid(HUDWidget))
		{
			HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
		}
	}

	/*
	if (LastActor.IsValid())
	{
		if (UActorComponent* HighlightableActorComponent = LastActor->FindComponentByInterface(UInv_Highlightable::StaticClass());
			IsValid(HighlightableActorComponent))
		{
			IInv_Highlightable::Execute_UnHighlight(HighlightableActorComponent);
		}
	}*/
}
