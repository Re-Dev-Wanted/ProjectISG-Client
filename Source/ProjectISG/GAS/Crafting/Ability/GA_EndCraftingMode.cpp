#include "GA_EndCraftingMode.h"

#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Systems/Animation/Manager/LevelSequenceManager.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "Task/AT_EndCraftingMode.h"

void UGA_EndCraftingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	AMainPlayerController* PC = Cast<AMainPlayerController>(
		Player->GetController());

	if (!Player)
	{
		OnEndCinematic();
		return;
	}

	AActor* TargetActor = const_cast<AActor*>(TriggerEventData->Target.Get());
	PC->PopUI();

	if (!TargetActor)
	{
		OnEndCinematic();
		return;
	}

	AWorkbench* Workbench = Cast<AWorkbench>(TargetActor);

	AT_EndCraftingModeCinematic = UAT_EndCraftingMode::InitialEvent(
		this, CraftingEndCinematic, Workbench);
	AT_EndCraftingModeCinematic->OnEndCraftingCinematicEndNotified.AddDynamic(
		this, &ThisClass::OnEndCinematic);
	AT_EndCraftingModeCinematic->ReadyForActivation();
}

void UGA_EndCraftingMode::OnEndCinematic()
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
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		PC->GetUIManageComponent()->ResetWidget();

		if (Player->GetInteractionComponent()->GetIsRestrictionTime() == false)
		{
			Player->GetInteractionComponent()->SetIsInteractive(true);
		}
		Player->GetPlacementIndicatorComponent()->SetIsActive(true);
	}

	Player->GetHandSlotComponent()->ToggleShowItem(true);

	AMainPlayerCharacter* _Player = const_cast<AMainPlayerCharacter*>(Player);
	_Player->StopAnimMontage(_Player->GetCurrentMontage());

	AActor* TargetActor = const_cast<AActor*>(CurrentEventData.Target.Get());
	AWorkbench* Workbench = Cast<AWorkbench>(TargetActor);

	if (Workbench)
	{
		Workbench->OnClosed();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
	           false);
}
