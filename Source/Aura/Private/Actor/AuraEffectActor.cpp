// Copyright - none


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	ActorWidget = CreateDefaultSubobject<UWidgetComponent>("ActorWidget");
	ActorWidget->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;
	constexpr float SinePeriod = 2 * PI;
	if (RunningTime >= SinePeriod)
	{
		RunningTime = 0.0f;
	}
	ItemMovement(DeltaTime);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
	CalculatedRotation = GetActorRotation();

	ActorWidget->SetVisibility(false);
}

void AAuraEffectActor::ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(EnemyTag) && !bApplyEffectsToEnemies)
	{
		return;
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr)
	{
		return;
	}

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	EGameplayEffectDurationType DurationPolicyVal = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy;
	if (DurationPolicyVal != EGameplayEffectDurationType::Infinite && bDestroyOnEffectApplication)
	{
		Destroy();
		return;
	}
	
	switch (DurationPolicyVal)
	{
	case EGameplayEffectDurationType::Infinite:
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetASC);
		break;
	case EGameplayEffectDurationType::Instant:
		{
			ActiveInstantEffectHandles.Add(ActiveGameplayEffectHandle);
		}
		break;
	case EGameplayEffectDurationType::HasDuration:
		ActiveDurationEffectHandles.Add(ActiveGameplayEffectHandle);
		break;
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantiateEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor, InstantiateGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantiateEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor, InstantiateGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}
	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandleToRemoves;
		for (const TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*>& HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandleToRemoves.Add(HandlePair.Key);
			}
		}
		for (const FActiveGameplayEffectHandle& Handle : HandleToRemoves)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}

void AAuraEffectActor::StartSinusoidalMovement()
{
	bSinusoidalMovement = true;
	InitialLocation = GetActorLocation();
	CalculatedLocation = InitialLocation;
}

void AAuraEffectActor::StartRotation()
{
	bRotates = true;
	CalculatedRotation = GetActorRotation();
}

void AAuraEffectActor::ShowActorWidget(bool bVisibility, AActor* TargetActor)
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

void AAuraEffectActor::ItemMovement(float DeltaTime)
{
	if (bRotates)
	{
		const FRotator DeltaRotation(0.0f, DeltaTime * RotationRate, 0.0f);
		CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);
	}
	if (bSinusoidalMovement)
	{
		const float Sine = SineAmplitude * FMath::Sin(RunningTime * SineConstant);
		CalculatedLocation = InitialLocation + FVector(0.0f, 0.0f, Sine);
	}
}

