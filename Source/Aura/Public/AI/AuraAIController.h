// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
public:
	AAuraAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
