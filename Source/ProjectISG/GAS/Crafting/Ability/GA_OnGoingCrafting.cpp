#include "GA_OnGoingCrafting.h"

#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Animation/Manager/AnimMontageManager.h"

void UGA_OnGoingCrafting::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	
	AT_StartMontageEvent = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::Crafting),
		FGameplayTagContainer()
	);

	AT_StartMontageEvent->OnCompleted.AddDynamic(this, &ThisClass::EndMontage);
	AT_StartMontageEvent->ReadyForActivation();
}

void UGA_OnGoingCrafting::EndMontage(FGameplayTag EventTag, FGameplayEventData Payload)
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>
		(CurrentActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}

	if (!CurrentEventData.Target)
	{
		return;
	}

	AMainPlayerController* PC = Cast<AMainPlayerController>( 
		Player->GetController());

	AMainPlayerCharacter* _Player = const_cast<AMainPlayerCharacter*>(Player);
	_Player->PlayAnimMontage(
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::Idle_OnTable)
	);

	if (Player->IsLocallyControlled())
	{
		PC->PushUI(EUIName::Popup_CraftingUI);

		UUIC_WorkbenchUI* UIController = Cast<UUIC_WorkbenchUI>
		(PC->GetUIManageComponent()->ControllerInstances[EUIName::Popup_CraftingUI]);

		AActor* TargetActor = const_cast<AActor*>(CurrentEventData.Target.Get());
		AWorkbench* Workbench = Cast<AWorkbench>(TargetActor);

		if (Workbench)
		{			
			UIController->SetUIHandler(Workbench);
		}
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
