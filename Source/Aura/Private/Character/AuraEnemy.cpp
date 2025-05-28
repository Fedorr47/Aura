// Copyright - none


#include "Character/AuraEnemy.h"

#include "AIController.h"
#include "AuraGameplayTags.h"
#include "Aura/Aura.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widgets/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCustomDepthStencilValue(HighlightChannel);
	GetMesh()->SetComponentTickEnabled(true);
	GetMesh()->SetUpdateAnimationInEditor(true);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	Weapon->SetCustomDepthStencilValue(HighlightChannel);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}
	
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(
			GameplayTags.Effects_HitReact_Fire, EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &ThisClass::HitReactTagChanged);
		
		OnHealthChanged.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAS->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}

	OnAbilitySystemComponentRegistrated.Broadcast(AbilitySystemComponent);
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(
		this,
		CharacterClass,
		Level,
		AbilitySystemComponent);
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	Weapon->SetRenderCustomDepth(true);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

UAnimInstance* AAuraEnemy::GetWeaponAnimInstance_Implementation()
{
	if (IsValid(Weapon))
	{
		return Weapon->GetAnimInstance();
	}
	return nullptr;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Die();
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	AuraAIController = Cast<AAuraAIController>(NewController);

	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}		


void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::HitReactTagChanged(CallbackTag, NewCount);
	if (IsValid(AuraAIController) && IsValid(AuraAIController->GetBlackboardComponent()))
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), true);
	}
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}
