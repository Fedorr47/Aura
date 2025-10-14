// Copyright - none


#include "EquipmentManagment/ProxyMesh/Inv_ProxyMesh.h"

#include "EquipmentManagment/Components/Inv_EquipmentComponent.h"
#include "GameFramework/Character.h"

AInv_ProxyMesh::AInv_ProxyMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	ProxySkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	ProxySkeletalMeshComponent->SetupAttachment(RootComponent);

	EquipmentComponent = CreateDefaultSubobject<UInv_EquipmentComponent>("Equipment");
	EquipmentComponent->SetOwningSkeletalMesh(ProxySkeletalMeshComponent);
	EquipmentComponent->SetIsProxy(true);
}

void AInv_ProxyMesh::BeginPlay()
{
	Super::BeginPlay();
	DelayedInitializeOwner();
}

void AInv_ProxyMesh::DelayedInitializeOwner()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		DelayedInitialization();
		return;
	}
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		DelayedInitialization();
		return;
	}
	ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn());
	if (!IsValid(Character))
	{
		DelayedInitialization();
		return;
	}
	USkeletalMeshComponent* CharacterSkeletalMesh = Character->GetMesh();
	if (!IsValid(CharacterSkeletalMesh))
	{
		DelayedInitialization();
		return;
	}
	SourceMesh = CharacterSkeletalMesh;
	ProxySkeletalMeshComponent->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	ProxySkeletalMeshComponent->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());
	
	///-----------------------------------------///
	auto Instances = GetLinkedAnimInstancesInternal(SourceMesh.Get());
	if (Instances.Num() > 0)
	{
		ProxySkeletalMeshComponent->LinkAnimClassLayers(Instances[0]->GetClass());
	}

	////----------------------------------------------------------/////
	EquipmentComponent->InitializeOwner(PlayerController);
}

void AInv_ProxyMesh::DelayedInitialization()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &AInv_ProxyMesh::DelayedInitializeOwner);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

const TArray<UAnimInstance*> AInv_ProxyMesh::GetLinkedAnimInstancesInternal(const USkeletalMeshComponent* Mesh) const
{
	auto Instances = Mesh->GetLinkedAnimInstances();
	return Instances;
}
