#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_AttachMesh.generated.h"

UCLASS()
class PROJECTISG_API UAnimNotify_AttachMesh : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = Property)
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> AttachedMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = Property)
	FName SocketName;
};
