// Copyright - none


#include "Widgets/CharacterDisplay/Inv_CharacterDisplay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "EquipmentManagment/ProxyMesh/Inv_ProxyMesh.h"
#include "Kismet/GameplayStatics.h"

FReply UInv_CharacterDisplay::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	LastMousePosition = CurrentMousePosition;
	
	bIsDragging = true;
	return FReply::Handled();
}

FReply UInv_CharacterDisplay::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return FReply::Handled();
}

void UInv_CharacterDisplay::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bIsDragging = false;
}

void UInv_CharacterDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TArray<AActor*> ProxyActors;
	
	UGameplayStatics::GetAllActorsOfClass(
		this, AInv_ProxyMesh::StaticClass(), ProxyActors);

	if (ProxyActors.Num() <= 0)
	{
		return;
	}

	AInv_ProxyMesh* ProxyMesh = Cast<AInv_ProxyMesh>(ProxyActors[0]);
	if (!IsValid(ProxyMesh))
	{
		return;
	}

	DisplayMesh = ProxyMesh->GetProxyMesh();
}

void UInv_CharacterDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsDragging)
	{
		return;
	}

	LastMousePosition = CurrentMousePosition;
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	const float HorizontalDelta = CurrentMousePosition.X - LastMousePosition.X;

	if (!DisplayMesh.IsValid())
	{
		return;
	}
	DisplayMesh->AddRelativeRotation(FRotator(0.0f, HorizontalDelta, 0.0f));
}
