#include "GA_CookingStartQTEEvent.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Cooking/UI/CookingQTE/UIC_CookingQTEUI.h"

void UGA_CookingStartQTEEvent::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	UBaseUIController* QTEPopupUI = Player->GetController<
		AMainPlayerController>()->GetUIManageComponent()->ControllerInstances[
		EUIName::Popup_CookingQTE];

	Cast<UUIC_CookingQTEUI>(QTEPopupUI);
}
