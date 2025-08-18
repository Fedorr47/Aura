// Copyright - none


#include "Actor/WeaponActor.h"

#include "Character/AuraCharacterBase.h"
#include "Combat/CombatComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/CombatInterface.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponSkeletalMeshComponent->SetupAttachment(GetRootComponent());

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(WeaponSkeletalMeshComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bReplicates = true;
}

USkeletalMesh* AWeaponActor::GetWeaponSkeletalMesh_Implementation()
{
	return WeaponSkeletalMeshComponent->GetSkeletalMeshAsset();
}

void AWeaponActor::SetWeaponSkeletalMesh_Implementation(USkeletalMesh* SkeletalMesh)
{
	WeaponSkeletalMeshComponent->SetSkeletalMesh(SkeletalMesh);
}

void AWeaponActor::SwapWeapon_Implementation(AActor* OwnerOfWeapon)
{
	// TODO Add all necessary swapping here
	if (OwnerOfWeapon->Implements<UCombatInterface>())
	{
		USkeletalMesh* SwapWeaponMesh = ICombatInterface::Execute_GetWeapon(OwnerOfWeapon)->GetSkeletalMeshAsset();
		IWeaponInterface::Execute_SetWeaponSkeletalMesh(this, SwapWeaponMesh);
		ReplicatedWeaponMesh = SwapWeaponMesh;
	}
}

void AWeaponActor::OnRep_WeaponMesh()
{
	IWeaponInterface::Execute_SetWeaponSkeletalMesh(this, ReplicatedWeaponMesh);
}

void AWeaponActor::OnOverlap(AActor* TargetActor)
{
	ShowActorWidget(true, TargetActor);
	AddPickableItemToActor(TargetActor);
}

void AWeaponActor::OnEndOverlap(AActor* TargetActor)
{
	ShowActorWidget(false, TargetActor);
	RemovePickableItemFromActor(TargetActor);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponActor::OnRep_IsEquipped()
{
	UE_LOG(LogTemp, Warning, TEXT("Actor: %s"),
	   *GetNameSafe(this));
}


void AWeaponActor::AddPickableItemToActor(AActor* Actor)
{
	if ( IsValid(Actor) && Actor->Implements<UCombatInterface>())
	{
		Cast<AAuraCharacterBase>(Actor)->AddPickableItem(this);
	}
}

void AWeaponActor::RemovePickableItemFromActor(AActor* Actor)
{
	if (IsValid(Actor) && Actor->Implements<UCombatInterface>())
	{
		Cast<AAuraCharacterBase>(Actor)->RemovePickableItem(this);
	}
}

void AWeaponActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AWeaponActor, ReplicatedWeaponMesh);
}

