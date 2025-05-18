#include "GA_EndCraftingMode.h"

#include "ProjectISG/Contents/Building/Props/Workbench.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "Task/AT_EndCraftingMode.h"

void UGA_EndCraftingMode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
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

	AT_EndCraftingModeCinematic = UAT_EndCraftingMode::InitialEvent(this, EndCraftingCinematic);
	AT_EndCraftingModeCinematic->OnEndCraftingCinematicEndNotified.AddDynamic(this, &ThisClass::OnEndCinematic);
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
	}

	AActor* TargetActor = const_cast<AActor*>(CurrentEventData.Target.Get());
	AWorkbench* Workbench = Cast<AWorkbench>(TargetActor);
	
	if (Workbench)
	{			
		Workbench->OnClosed();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
