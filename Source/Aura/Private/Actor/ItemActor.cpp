// Copyright - none


#include "Actor/ItemActor.h"

#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/PlayerState.h"
#include "Interaction/CombatInterface.h"


AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ActorWidget = CreateDefaultSubobject<UWidgetComponent>("ActorWidget");
	ActorWidget->SetupAttachment(GetRootComponent());
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	ActorWidget->SetVisibility(false);
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemActor::ShowActorWidget(bool bVisibility, AActor* TargetActor)
{
	if (IsValid(ActorWidget) && TargetActor->Implements<UCombatInterface>())
	{
		if ((Cast<AAuraCharacterBase>(TargetActor)->HasAuthority() && Cast<AAuraCharacterBase>(TargetActor)->IsLocallyControlled())
			||
			(Cast<AAuraCharacterBase>(TargetActor)->GetPlayerState() &&
				Cast<AAuraCharacterBase>(TargetActor)->GetPlayerState()->GetPawn()
				&& Cast<AAuraCharacterBase>(TargetActor)->GetPlayerState()->GetPawn()->GetLocalRole() == ROLE_AutonomousProxy))
		{
			ActorWidget->SetVisibility(bVisibility);
		}
	}
}

void AItemActor::OnOverlap(AActor* TargetActor)
{
}

void AItemActor::OnEndOverlap(AActor* TargetActor)
{
}

