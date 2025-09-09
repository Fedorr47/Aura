// Copyright - none


#include "Items/Inv_Item.h"


AInv_Item::AInv_Item()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInv_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInv_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

