// Copyright - none


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(
				DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
					this, &UDebuffNiagaraComponent::DebuffDamageTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnAbilitySystemComponentRegistratedDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* InASC)
			{
				InASC->RegisterGameplayTagEvent(
					DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
					this, &UDebuffNiagaraComponent::DebuffDamageTagChanged);
			});
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffDamageTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
