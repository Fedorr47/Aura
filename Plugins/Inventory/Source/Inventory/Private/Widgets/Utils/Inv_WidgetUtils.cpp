// Copyright - none


#include "Widgets/Utils/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(
		Widget,
		Geometry,
		USlateBlueprintLibrary::GetLocalTopLeft(Geometry),
		PixelPosition,
		ViewportPosition);
	
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UInv_WidgetUtils::IsWithinBounds(const FVector2D& BoundaryPosition, const FVector2D& WidgetSize,
                                      const FVector2D& MousePosition)
{
	return MousePosition.X >= BoundaryPosition.X
		&& MousePosition.X <= (BoundaryPosition.X + WidgetSize.X)
		&& MousePosition.Y >= BoundaryPosition.Y
		&& MousePosition.Y <= (BoundaryPosition.Y + WidgetSize.Y);
}

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& BoundaryPosition, const FVector2D& WidgetSize,
	const FVector2D& MousePosition)
{
	FVector2D ClampedPosition = MousePosition;

	if (MousePosition.X + WidgetSize.X > BoundaryPosition.X)
	{
		ClampedPosition.X = BoundaryPosition.X - WidgetSize.X;
	}
	if (MousePosition.X < 0.0f)
	{
		ClampedPosition.X = 0.0f;
	}
	if (MousePosition.Y + WidgetSize.Y > BoundaryPosition.Y)
	{
		ClampedPosition.Y = BoundaryPosition.Y - WidgetSize.Y;
	}
	if (MousePosition.Y < 0.0f)
	{
		ClampedPosition.Y = 0.0f;
	}
	
	return ClampedPosition;
}

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Columns * Position.Y;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}
