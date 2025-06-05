// Copyright - none

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BurnDebuffEffect = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffEffect");
	BurnDebuffEffect->SetupAttachment(GetRootComponent());
	BurnDebuffEffect->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandleSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag)
{
	if (HasAuthority())
	{
		for (auto SocketPair : TagToWeaponTipSocketInfo)
		{
			if (SocketPair.Key.MatchesTagExact(SocketTag))
			{
				auto SocketOwner = GetSkeletalSocketOwner(SocketPair.Value.SocketType);
				if (IsValid(SocketOwner))
				{
					return SocketOwner->GetSocketTransform(SocketPair.Value.SocketName, ERelativeTransformSpace::RTS_World).GetLocation();
				}
			}
		}
	}
	return FVector::ZeroVector;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation(const FGameplayTag HitTag)
{
	for (auto HitReact : HitMontages)
	{
		if (HitReact.MontageTag.MatchesTagExact(HitTag))
		{
			return HitReact.Montage;
		}
	}
	return nullptr;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontage_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetEffectByTag_Implementation(const FGameplayTag& Tag)
{
	for (auto EffectPair : EffectsToTag)
	{
		if (EffectPair.Key.MatchesTagExact(Tag))
		{
			return EffectPair.Value;
		}
	}
	return nullptr;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& Tag)
{
	auto TaggedMontages = HitMontages;
	TaggedMontages.Append(AttackMontages);
	for (auto TaggedMontage : TaggedMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(Tag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::SetMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnAbilitySystemComponentRegistrated AAuraCharacterBase::GetOnAbilitySystemComponentRegistratedDelegate()
{
	return OnAbilitySystemComponentRegistrated;
}

FOnDeath AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

void AAuraCharacterBase::Knockback(const FVector& KnockbackImpulse)
{
	LaunchCharacter(KnockbackImpulse, true, true);
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

bool AAuraCharacterBase::IsHitReacting()
{
	return bHitReacting;
}

void AAuraCharacterBase::SetHitReactMontages(TArray<FTaggedMontage> InHitMontages)
{
	HitMontages = InHitMontages;
}

void AAuraCharacterBase::SetAttackMontages(TArray<FTaggedMontage> InAttackMontages)
{
	AttackMontages = InAttackMontages;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Dissolve();

	bDead = true;

	OnDeath.Broadcast(this);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

void AAuraCharacterBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? (MaxWalkSpeed/2.0f) : MaxWalkSpeed;
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(GameplayEffectClass);
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, Level,EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!HasAuthority()) return;

	AuraASC->AddCharacterAbilities(GrantedAbilities);
	AuraASC->AddCharacterPassiveAbilities(GrantedPassiveAbilities);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(
			DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMaterialInstance);
		StartDissolveTimeLine(DynamicMaterialInstance);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMaterialInstance = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, WeaponDynamicMaterialInstance);
		StartWeaponDissolveTimeLine(WeaponDynamicMaterialInstance);
	}
}

USceneComponent* AAuraCharacterBase::GetSkeletalSocketOwner(ESkeletalSocketType InSkeletalSocketType)
{
	switch (InSkeletalSocketType)
	{
	case ESkeletalSocketType::WeaponSocket:
		return Weapon;
		break;
	case ESkeletalSocketType::MeshSocket:
		[[fallthrough]];
		default:
			return GetMesh();
	}
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.0f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.0f);
}



