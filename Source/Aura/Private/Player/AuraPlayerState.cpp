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
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
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

void AAuraPlayerState::AddAttributePoints(const int32 NewAmount)
{
	AttributePoints += NewAmount;
	OnAttributesPointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddSpellPoints(const int32 NewAmount)
{
	SpellPoints += NewAmount;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetAttributePoints(const int32 NewAmount)
{
	AttributePoints = NewAmount;
	OnAttributesPointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(const int32 NewAmount)
{
	SpellPoints = NewAmount;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_ExperiencePoints(int32 OldExperiencePoints)
{
	OnExperiencePointsChangedDelegate.Broadcast(ExperiencePoints);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributesPointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
