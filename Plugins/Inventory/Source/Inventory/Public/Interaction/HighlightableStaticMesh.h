// Copyright - none

#pragma once

#include "CoreMinimal.h"
#include "Inv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "HighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORY_API UHighlightableStaticMesh : public UStaticMeshComponent, public IInv_Highlightable
{
	GENERATED_BODY()
public:
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;

private:
	UPROPERTY(EditAnywhere, Category=Inventory)
	TObjectPtr<UMaterialInterface> HighlightMaterialInterface;
};
