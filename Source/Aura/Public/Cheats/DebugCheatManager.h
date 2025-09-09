// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "DebugCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDebugCheatManager : public UCheatManager
{
	GENERATED_BODY()
public:

	virtual void God() override;
	
	UFUNCTION(exec)
	void KillAll(const FString& ClassName);

	UFUNCTION(exec,BlueprintCallable,Category="Debug Cheat Manager")
	void UnlimitedMana(bool NewValue);
	
private:
	APawn* GetOwnerPawn();
};
