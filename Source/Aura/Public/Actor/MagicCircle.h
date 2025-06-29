// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()
public:	
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	UDecalComponent* GetMagicCircleDecal() { return MagicCircleDecal;};

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;
};
