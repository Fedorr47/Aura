// Copyright - none


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.0f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, ExperiencePoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetLevel(const int32 NewAmount)
{
	Level = NewAmount;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToLevel(const int32 NewAmount)
{
	Level += NewAmount;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetExperiencePoints(const int32 NewAmount)
{
	ExperiencePoints = NewAmount;
	OnExperiencePointsChangedDelegate.Broadcast(ExperiencePoints);
}

void AAuraPlayerState::AddExperiencePoints(const int32 NewAmount)
{
	ExperiencePoints += NewAmount;
	OnExperiencePointsChangedDelegate.Broadcast(ExperiencePoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(OldLevel);
}

void AAuraPlayerState::OnRep_ExperiencePoints(int32 OldExperiencePoints)
{
	OnExperiencePointsChangedDelegate.Broadcast(OldExperiencePoints);
}