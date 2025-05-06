// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UBehaviorTree;
class AAuraAIController;

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AAuraEnemy();

	/* Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/* End Enemy Interface */

	/* Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface */

	virtual void Die() override;

	virtual void PossessedBy(AController* NewController) override;

	//-----------------------------------------------------///

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float LifeSpan{5.0f};
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	//-----------------------------------------------------------------------//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Class Defaults")
	int32 Level{1};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Class Defaults")
	ECharacterClass CharacterClass{ECharacterClass::Magic};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AAuraAIController> AuraAIController;
};
