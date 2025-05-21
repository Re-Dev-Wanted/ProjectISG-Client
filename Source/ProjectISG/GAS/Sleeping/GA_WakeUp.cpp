// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WakeUp.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Animation/Manager/AnimMontageManager.h"
#include "ProjectISG/Systems/Time/Bed.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_WakeUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                 const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo
                                 ActivationInfo,
                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("침대에서 일어나기 어빌리티, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));

	AT_WakeUpAnim = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::Sleep_WakeUp),
		FGameplayTagContainer(),
		CurrentEventData);

	AT_WakeUpAnim->Activate();
	AT_WakeUpAnim->OnCompleted.AddDynamic(
		this, &ThisClass::OnCompleteWakeUpMontage);
}

void UGA_WakeUp::EndAbility(const FGameplayAbilitySpecHandle Handle,
                            const FGameplayAbilityActorInfo* ActorInfo,
                            const FGameplayAbilityActivationInfo ActivationInfo,
                            bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_WakeUp::OnCompleteWakeUpMontage(FGameplayTag EventTag,
                                         FGameplayEventData EventData)
{
	UE_LOG(LogTemp, Warning, TEXT("몽타주 끝"))
	BlockInputForMontage(false);
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		CurrentActorInfo->AvatarActor.Get());
	Player->bUseControllerRotationYaw = true;

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		Player->GetController());
	PC->SetViewTargetWithBlend(Player);

	if (CurrentEventData.Target)
	{
		const AActor* Target = CurrentEventData.Target.Get();
		const ABed* Bed = Cast<ABed>(Target);
		Bed->SetCollisionEnabled(true);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}
