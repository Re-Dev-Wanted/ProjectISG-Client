#include "AnimNotify_AttachMesh.h"

void UAnimNotify_AttachMesh::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp)
	{
		// if (!AttachedMeshComponent)
		// {
		// 	AttachedMeshComponent = NewObject<UStaticMeshComponent>(this);
		// 	AttachedMeshComponent->RegisterComponent();
		// }
		// AttachedMeshComponent->SetStaticMesh(StaticMesh);
		// AttachedMeshComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

void UAnimNotify_AttachMesh::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// if (AttachedMeshComponent)
	// {
	// 	AttachedMeshComponent->DestroyComponent();
	// 	AttachedMeshComponent = nullptr;
	// }
}
