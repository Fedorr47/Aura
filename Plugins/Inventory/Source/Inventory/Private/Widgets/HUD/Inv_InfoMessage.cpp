// Copyright - none


#include "Widgets/HUD/Inv_InfoMessage.h"

#include "Components/TextBlock.h"

void UInv_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Text_Message->SetText(FText::GetEmpty());
	MessageHide();
}

void UInv_InfoMessage::SetMessage(const FText& Message)
{
	Text_Message->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}

	GetWorld()->GetTimerManager().SetTimer(
		MessageTimerHandle,
		[this]()
	{
		MessageHide();
		bIsMessageActive = false;
	},
	MessageLifeTime,
	false);
}
