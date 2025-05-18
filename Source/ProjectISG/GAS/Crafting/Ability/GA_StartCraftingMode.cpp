#include "GA_StartCraftingMode.h"

#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Crafting/UI/UIC_WorkbenchUI.h"
#include "Task/AT_StartCraftingMode.h"

void UGA_StartCraftingMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	if (!Player)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	TObjectPtr<ABaseActor> HeldItem = Player->GetHandSlotComponent()->GetHeldItem();

	if (HeldItem)
	{
		HeldItem->SetHidden(true);
	}

	AMainPlayerController* PC = Player->GetController<AMainPlayerController>();
	
	if (!PC)
	{
		OnEndCinematic();
		return;
	}

	PC->SetIgnoreMoveInput(true);
	PC->SetIgnoreLookInput(true);

	AT_StartCraftingModeCinematic = UAT_StartCraftingMode::InitialEvent(this, CraftingReadyCinematic);
	AT_StartCraftingModeCinematic->OnStartCraftingCinematicEndNotified.AddDynamic(this, &ThisClass::OnEndCinematic);
	AT_StartCraftingModeCinematic->ReadyForActivation();
}

void UGA_StartCraftingMode::OnEndCinematic()
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
