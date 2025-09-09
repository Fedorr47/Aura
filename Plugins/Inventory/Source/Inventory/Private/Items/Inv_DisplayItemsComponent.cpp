// Copyright - none


#include "Items/Inv_DisplayItemsComponent.h"


UInv_DisplayItemsComponent::UInv_DisplayItemsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PickupMessage = FString("Pick Up");
}


void UInv_DisplayItemsComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UInv_DisplayItemsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

