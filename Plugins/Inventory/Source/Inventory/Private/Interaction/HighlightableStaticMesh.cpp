// Copyright - none


#include "Interaction/HighlightableStaticMesh.h"

void UHighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterialInterface);
}

void UHighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
