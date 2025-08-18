// Copyright - none


#include "UI/HUD/OverHeadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString InText)
{
	if (PlayerNameText)
	{
		PlayerNameText->SetText(FText::FromString(InText));
	}
}

void UOverHeadWidget::ShowPlayerName(APawn* InPawn)
{
	if (IsValid(InPawn))
	{
		ENetRole LocalRole = InPawn->GetLocalRole();
		FString Role = StaticEnum<ENetRole>()->GetNameStringByValue(static_cast<int64>(LocalRole));
		SetDisplayText(Role);
	}
}

void UOverHeadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
