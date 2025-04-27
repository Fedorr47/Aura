// Copyright - none


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CharacterCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll= false;
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::Look(const FInputActionValue& Value)
{
	float MinPitch = -80.f;
	float MaxPitch = -30.f;
	
	FVector2D LookInput = Value.Get<FVector2D>();

	FRotator NewRotation = CameraBoom->GetRelativeRotation();
	NewRotation.Yaw += LookInput.X;
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + LookInput.Y, MinPitch, MaxPitch);

	CameraBoom->SetRelativeRotation(NewRotation);
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}
