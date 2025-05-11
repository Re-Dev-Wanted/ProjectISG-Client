// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_OpenTradingUI.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Utils/EnumUtil.h"

class AMainPlayerCharacter;

void UGA_OpenTradingUI::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo*
                                        ActorInfo,
                                        const FGameplayAbilityActivationInfo
                                        ActivationInfo,
                                        const FGameplayEventData*
                                        TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	UE_LOG(LogTemp, Warning, TEXT("Open Trading UI, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));

	Player->GetController<AMainPlayerController>()->PopUI();
	Player->GetController<AMainPlayerController>()->PushUI(
		EUIName::Popup_TradingUI);

	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}

void UGA_OpenTradingUI::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo, bool bReplicateEndAbility,
                                   bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}
