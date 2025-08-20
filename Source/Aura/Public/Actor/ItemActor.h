// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_Item.h"
#include "ItemActor.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API AItemActor : public AInv_Item
{
	GENERATED_BODY()

public:
	AItemActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ShowActorWidget(bool bVisibility, AActor* TargetActor) override;
	
	virtual void OnOverlap(AActor* TargetActor) override;
	virtual void OnEndOverlap(AActor* TargetActor) override;
};
