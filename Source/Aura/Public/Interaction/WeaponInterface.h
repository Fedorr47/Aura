// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class AURA_API IWeaponInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMesh* GetWeaponSkeletalMesh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWeaponSkeletalMesh(USkeletalMesh* SkeletalMesh);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemoveFromLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DropOnLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SwapWeapon(AActor* OwnerOfWeapon);
};


