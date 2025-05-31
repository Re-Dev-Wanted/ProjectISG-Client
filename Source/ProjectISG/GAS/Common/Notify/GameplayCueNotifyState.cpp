#include "GameplayCueNotifyState.h"

#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/BaseCharacter.h"

void UGameplayCueNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                          UAnimSequenceBase* Animation,
                                          float TotalDuration,
                                          const FAnimNotifyEventReference&
                                          EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(
		MeshComp->GetOwner()))
	{
		FGameplayCueParameters Param;
		Param.AbilityLevel = 1;
		Param.EffectCauser = BaseCharacter;

		BaseCharacter->GetAbilitySystemComponent()->ExecuteGameplayCue(
			CueTag, Param);
	}
}

void UGameplayCueNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                        UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference&
                                        EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (const ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(
		MeshComp->GetOwner()))
	{
		BaseCharacter->GetAbilitySystemComponent()->RemoveGameplayCue(
			CueTag);
	}
}
