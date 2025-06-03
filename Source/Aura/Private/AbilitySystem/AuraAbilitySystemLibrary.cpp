// Copyright - none


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "Character/AuraCharacterBase.h"
#include "GameModes/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	return GetWidgetController<UOverlayWidgetController>(WorldContextObject, &AAuraHUD::GetOverlayWidgetController);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeWidgetController(
	const UObject* WorldContextObject)
{
	return GetWidgetController<UAttributeMenuWidgetController>(WorldContextObject,
	                                                           &AAuraHUD::GetAttributeWidgetController);
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	return GetWidgetController<USpellMenuWidgetController>(WorldContextObject, &AAuraHUD::GetSpellMenuWidgetController);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(
	const UObject* WorldContextObject,
	ECharacterClass CharacterClass,
	float Level,
	UAbilitySystemComponent* ASC)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		AActor* AvatarActor = ASC->GetAvatarActor();
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		FGameplayEffectContextHandle PrimaryAttributesCxtHandle = ASC->MakeEffectContext();
		PrimaryAttributesCxtHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
			ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesCxtHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

		FGameplayEffectContextHandle SecondaryAttributesCxtHandle = ASC->MakeEffectContext();
		SecondaryAttributesCxtHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(
			ClassDefaultInfo.SecondaryAttributes, Level, SecondaryAttributesCxtHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

		FGameplayEffectContextHandle VitalAttributesCxtHandle = ASC->MakeEffectContext();
		VitalAttributesCxtHandle.AddSourceObject(AvatarActor);
		const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(
			ClassDefaultInfo.VitalAttributes, Level, VitalAttributesCxtHandle);
		ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	}
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(
	const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC,
	ECharacterClass CharacterClass /*= ECharacterClass::Magic*/)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
			ASC->GiveAbility(AbilitySpec);
		}
		if (CharacterClass == ECharacterClass::Player)
		{
			return;
		}
		const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
		{
			if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(
					AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGameModeBase->CharacterClassInfo;
	}
	return nullptr;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(
		UGameplayStatics::GetGameMode(WorldContextObject)))
	{
		return AuraGameModeBase->AbilityInfo;
	}
	return nullptr;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->IsBlockedHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetBlockedHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->IsSuccessfulDebuff();
	}
	return false;
}

void UAuraAbilitySystemLibrary::SetSuccessfulDebuff(FGameplayEffectContextHandle& ContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->GetDebuffDamage();
	}
	return 0.0f;
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& ContextHandle, float InDebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffDamage(InDebuffDamage);
	}
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->GetDebuffDuration();
	}
	return 0.0f;
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& ContextHandle,
														 float InDebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffDuration(InDebuffDuration);
	}
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		return AuraContext->GetDebuffFrequency();
	}
	return 0.0f;
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& ContextHandle,
														  float InDebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
		Get()))
	{
		if (AuraContext->GetDamageType().IsValid())
		{
			return *AuraContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& ContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);;
		AuraContext->SetDamageType(DamageType);
	}
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
			Get()))
	{
		return AuraContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& ContextHandle, const FVector& InDeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetDeathImpulse(InDeathImpulse);
	}
}

FVector UAuraAbilitySystemLibrary::GetKnockbackImpulse(const FGameplayEffectContextHandle& ContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(ContextHandle.
			Get()))
	{
		return AuraContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetKnockbackImpulse(FGameplayEffectContextHandle& ContextHandle,
	const FVector& InKnockbackImpulse)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(ContextHandle.Get()))
	{
		AuraContext->SetKnockbackImpulse(InKnockbackImpulse);
	}
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithRadius(
	const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore,
	float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			const bool IsCombatActor = OverlapResult.GetActor()->Implements<UCombatInterface>();
			if (IsCombatActor && !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
	//UGameplayStatics::ApplyRadialDamageWithFalloff()
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool bSecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool bFirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool bSecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));
	const bool bIsFriends = (bFirstIsPlayer && bSecondIsPlayer) || (bFirstIsEnemy && bSecondIsEnemy);
	return !bIsFriends;
}

int32 UAuraAbilitySystemLibrary::GetReward(const UObject* WorldContextObject, ECharacterClass InCharacterClass,
                                           int32 InLevel)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		FString EnumValueAsString = StaticEnum<ECharacterClass>()->GetNameStringByValue(
			static_cast<uint8>(InCharacterClass));
		const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(InCharacterClass);

		return static_cast<int32>(ClassDefaultInfo.ExperienceReward.GetValueAtLevel(InLevel));
	}
	return 0;
}

FGameplayEffectContextHandle UAuraAbilitySystemLibrary::DoDamage(const FDamageEffectParam& DamageEffectParam)
{
	const AActor* SourceAvatarActor = DamageEffectParam.SourceAbilitySystemComponent->GetAvatarActor();
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FGameplayEffectContextHandle ContextHandle = DamageEffectParam.SourceAbilitySystemComponent->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(ContextHandle, DamageEffectParam.DeathImpulse);
	SetKnockbackImpulse(ContextHandle, DamageEffectParam.KnockbackImpulse);
	
	FGameplayEffectSpecHandle SpecHandle = DamageEffectParam.SourceAbilitySystemComponent->MakeOutgoingSpec(
		DamageEffectParam.DamageGameplayEffectClass,
		DamageEffectParam.AbilityLevel,
		ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, DamageEffectParam.DamageType, DamageEffectParam.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParam.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParam.DebufChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParam.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParam.DebuffFrequency);

	DamageEffectParam.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	return ContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpaceRotators(
	const FVector&
	Forward,
	const FVector& Axis,
	float SpreadAngle,
	int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpreadAngle/2.0f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = SpreadAngle / (NumRotators-1);
		for (int32 i = 0; i < NumRotators; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVectors(
	const FVector& Forward,
	const FVector& Axis,
	float SpreadAngle,
	int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpreadAngle/2.0f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = SpreadAngle / (NumVectors-1);
		for (int32 i = 0; i < NumVectors; ++i)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}
