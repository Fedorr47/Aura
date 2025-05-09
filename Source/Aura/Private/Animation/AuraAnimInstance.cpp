// Copyright - none


#include "Animation/AuraAnimInstance.h"

#include "Character/AuraCharacterBase.h"
#include "GameFramework/Character.h"

FTransform UAuraAnimInstance::TryGetSocketTransformFromMeshOwner() const
{
	USkeletalMeshComponent* OwnerComponent = GetSkelMeshComponent();
	if (AActor* OwnerActor = OwnerComponent->GetOwner())
	{
		if (ACharacter* Character = Cast<ACharacter>(OwnerActor) )
		{
			if (USkeletalMeshComponent* MeshComp = Character->GetMesh())
			{
				return MeshComp->GetSocketTransform(FName("RightHandSocket"), ERelativeTransformSpace::RTS_World);
			}
		}
		
	}

	return FTransform();
}
