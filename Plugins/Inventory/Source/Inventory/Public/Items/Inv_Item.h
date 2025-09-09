// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_Item.generated.h"

class UWidgetComponent;

UCLASS()
class INVENTORY_API AInv_Item : public AActor
{
	GENERATED_BODY()

public:
	AInv_Item();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Widget")
	TObjectPtr<UWidgetComponent> ActorWidget;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ShowActorWidget(bool bVisibility, AActor* TargetActor) {};

	UFUNCTION(BlueprintCallable)
	virtual void OnOverlap(AActor* TargetActor) {};

	UFUNCTION(BlueprintCallable)
	virtual void OnEndOverlap(AActor* TargetActor) {};
};
