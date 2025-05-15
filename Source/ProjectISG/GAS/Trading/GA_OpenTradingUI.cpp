#include "GA_OpenTradingUI.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"

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
