// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class AURA_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetDisplayText(FString InText);

	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(APawn* InPawn);
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* PlayerNameText;

protected:
	virtual void NativeDestruct() override;
};
