#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayCueNotifyState.generated.h"

UCLASS()
class PROJECTISG_API UGameplayCueNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
	                         UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference&
	                         EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
	                       UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference&
	                       EventReference) override;

private:
	UPROPERTY(EditInstanceOnly, Category = "Options",
		meta = (AllowPrivateAccess = true))
	FGameplayTag CueTag;
};
