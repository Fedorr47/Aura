// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inv_ProxyMesh.generated.h"

class UInv_EquipmentComponent;
class USkeletalMeshComponent;

UCLASS()
class INVENTORY_API AInv_ProxyMesh : public AActor
{
	GENERATED_BODY()

public:
	AInv_ProxyMesh();

protected:
	virtual void BeginPlay() override;
private:

	void DelayedInitializeOwner();
	void DelayedInitialization();
	const TArray<UAnimInstance*> GetLinkedAnimInstancesInternal(const USkeletalMeshComponent* Mesh) const;
//--------------------------------------------------------------------------------------------------------------------//	
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInv_EquipmentComponent> EquipmentComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ProxySkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Inv_ProxyMesh")
	TSubclassOf<UAnimInstance>  ProxyAnimInstance;

	FTimerHandle TimerForNextTickHandle;
};
