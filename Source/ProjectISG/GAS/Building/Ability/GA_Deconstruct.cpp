// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Deconstruct.h"

#include "Kismet/KismetSystemLibrary.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"

void UGA_Deconstruct::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo,
                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}
	
	if (!Player->HasAuthority())
	{
		return;
	}

	if (Player->GetHandSlotComponent() && Player->GetInteractionComponent())
	{
		FHitResult TargetResult = Player->GetInteractionComponent()->GetTargetTraceResult();

		UKismetSystemLibrary::PrintString(GetWorld(), "???");

		if (TargetResult.GetActor())
		{
			Player->GetHandSlotComponent()->OnTouchAction(TargetResult.GetActor());
		}
	}
}

void UGA_Deconstruct::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
