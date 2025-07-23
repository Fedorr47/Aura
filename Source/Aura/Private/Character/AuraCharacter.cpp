// Copyright - none


#include "Character/AuraCharacter.h"


#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameModes/AuraGameModeBase.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->SetRelativeRotation(FRotator(-60.0f, -50.0f, 0.0f));
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	LevelUpEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpEffect"));
	LevelUpEffect->SetupAttachment(GetRootComponent());
	LevelUpEffect->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetMesh()->SetUpdateAnimationInEditor(true);
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	
	GetMesh()->SetCustomDepthStencilValue(PlayerStencilChannel);
	GetMesh()->SetRenderCustomDepth(true);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll= false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnAbilitySystemComponentRegistrated.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Shock, EGameplayTagEventType::NewOrRemoved).AddUObject(
		this, &ThisClass::ShockTagChanged);
	
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitHUD(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	
}

void AAuraCharacter::MulticastLevelUpEffect_Implementation()
{
	if (IsValid(LevelUpEffect))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector NiagaraLocation = LevelUpEffect->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
		LevelUpEffect->SetWorldRotation(ToCameraRotation);
		LevelUpEffect->Activate(true);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpEffect();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	// !!! Now works only for local version
	if (HasAuthority())
	{
		LoadProgress();
	}
	else
	{
		AddCharacterAbilities();
	}
	
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameModeBase->LoadWorldState(GetWorld());
	}
}

void AAuraCharacter::LoadProgress()
{
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveDatta();
		if (SaveData == nullptr)
		{
			return;
		}
		
		if (SaveData->bFirstTimeLoading)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
			{
				AuraASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			
			if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState<AAuraPlayerState>()))
			{
				AuraPlayerState->SetLevel(SaveData->PlayerLevel);
				AuraPlayerState->SetExperiencePoints(SaveData->ExperiencePoints);
				AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
				AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			}
			UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	if (AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (!HasAuthority()) return;
		
		ULoadScreenSaveGame* SaveData = AuraGameModeBase->RetrieveInGameSaveDatta();
		if (SaveData == nullptr)
		{
			return;
		}

		SaveData->PlayerStartTag = CheckpointTag;

		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState<AAuraPlayerState>()))
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->ExperiencePoints = AuraPlayerState->GetExperiencePoints();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
			SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		}

		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		SaveData->bFirstTimeLoading = false;

		
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
		FForEachAbility SaveAbilityDelegate;
		SaveData->SaveAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, AuraASC, &SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
			FAuraAbilityInfo AuraAbilityInfo = AbilityInfo->FindAbilityInfoByTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = AuraAbilityInfo.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = AuraASC->GetInputSlotTagFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = AuraAbilityInfo.AbilityType;
			
			SaveData->SaveAbilities.AddUnique(SavedAbility);
		});

		AuraASC->ForEachAbility(SaveAbilityDelegate);
		
		AuraGameModeBase->SaveInGameProgressData(SaveData);
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
	
	if (const UAuraAttributeSet* AuraAS = CastChecked<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
				FAuraGameplayTags::Get().Effects_HitReact_Fire, EGameplayTagEventType::NewOrRemoved).AddUObject(
					this, &ThisClass::HitReactTagChanged);
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::AddToExperiencePoints_Implementation(int32 InExperiencePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddExperiencePoints(InExperiencePoints);
}

int32 AAuraCharacter::GetExperiencePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->GetExperiencePoints();
}

int32 AAuraCharacter::FindLevelForExperiencePoints_Implementation(int32 InExperiencePoints) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	
	return AuraPlayerState->LevelUpInfo->FindLevelUpInfoByXP(InExperiencePoints);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	if (Level < AuraPlayerState->LevelUpInfo->LevelUpInformation.Num())
		return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointsReward;
	return 0;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	if (Level < AuraPlayerState->LevelUpInfo->LevelUpInformation.Num())
		return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
	return 0;
}

void AAuraCharacter::AddPlayerLevel_Implementation(int32 InPlayerLevels)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddToLevel(InPlayerLevels);

	if (UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilitiesStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddSpellPoints_Implementation(int32 SpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AuraPlayerState->AddSpellPoints(SpellPoints);
}

inline int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetAttributePoints();
}

inline int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation()
{
	AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>();
	check(AuraPlayerController);

	AuraPlayerController->ShowMagicCircle();
	AuraPlayerController->bShowMouseCursor = false;
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>();
	check(AuraPlayerController);

	AuraPlayerController->HideMagicCircle();
	AuraPlayerController->bShowMouseCursor = true;
}

void AAuraCharacter::SetMagicCircleMaterial_Implementation(UMaterialInterface* InMaterial)
{
	AAuraPlayerController* AuraPlayerController = GetController<AAuraPlayerController>();
	check(AuraPlayerController);

	AuraPlayerController->SetMagicCircleMaterial(InMaterial);
}

void AAuraCharacter::OnRep_Shocked()
{
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		if (bIsStunned)
		{
			AuraAbilitySystemComponent->AddLooseGameplayTags(BlockedTags);
			StunDebuffEffect->Activate();
		}
		else
		{
			AuraAbilitySystemComponent->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffEffect->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffEffect->Activate();
	}
	else
	{
		BurnDebuffEffect->Deactivate();
	}
}
