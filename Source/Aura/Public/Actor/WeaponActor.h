// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Actor/ItemActor.h"
#include "Interaction/WeaponInterface.h"
#include "WeaponActor.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponStatus : uint8
{
	Available UMETA(DisplayName = "Available"),
	Equipped UMETA(DisplayName = "Equipped"),
	
	MAX UMETA(DisplayName = "MAX")
};

UCLASS()
class AURA_API AWeaponActor : public AItemActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	AWeaponActor();

	/* Weapon Interface*/
	USkeletalMesh* GetWeaponSkeletalMesh_Implementation() override;
	void SetWeaponSkeletalMesh_Implementation(USkeletalMesh* SkeletalMesh) override;
	void SwapWeapon_Implementation(AActor* OwnerOfWeapon) override;
	/* End Weapon Interface */

	void OnOverlap(AActor* TargetActor) override;
	void OnEndOverlap(AActor* TargetActor) override;

	UFUNCTION(BlueprintCallable)
	void AddPickableItemToActor(AActor* Actor);

	UFUNCTION(BlueprintCallable)
	void RemovePickableItemFromActor(AActor* Actor);

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_WeaponMesh();

//--------------------------------------------------------------------------------------------------------------------//
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMeshComponent;

	UPROPERTY(ReplicatedUsing=OnRep_WeaponMesh)
	TObjectPtr<USkeletalMesh> ReplicatedWeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(BlueprintReadWrite)
	EWeaponStatus WeaponStatus{EWeaponStatus::Available};
};
