#include "GA_EndSitDown.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/UI/Base/Interfaces/UIHandler.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Animation/Manager/AnimMontageManager.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"
#include "ProjectISG/Systems/Grid/Components/PlacementIndicatorComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"

void UGA_EndSitDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                     const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get());

	if (TriggerEventData->Target)
	{
		const AActor* Target = TriggerEventData->Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);
		ConstPlacement->Multicast_SetCollisionEnabled(false);

		APlacement* Placement = const_cast<APlacement*>(ConstPlacement);
		
		FVector Point = Placement->GetEndInteractPoint();
		FRotator Rotation = Placement->GetEndInteractRotation();
		
		Player->SetActorLocation(Point);
		Player->GetController()->SetControlRotation(Rotation);
		Player->SetActorRotation(Rotation);
	}

	AT_EndMontageEvent = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::SitDown_End),
		FGameplayTagContainer(),
		*TriggerEventData
	);

	AT_EndMontageEvent->OnCompleted.AddDynamic(this, &UGA_EndSitDown::EndMontage);

	AT_EndMontageEvent->ReadyForActivation();
}

void UGA_EndSitDown::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_EndSitDown::EndMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	
	if (!Player)
	{
		return;
	}

	if (Player->IsLocallyControlled())
	{
		Player->bUseControllerRotationYaw = true;
		Player->GetCharacterMovement()->bOrientRotationToMovement = false;
		Player->GetInteractionComponent()->SetIsInteractive(true);
		Player->GetPlacementIndicatorComponent()->SetIsActive(true);
	}

	Player->GetHandSlotComponent()->ToggleShowItem(true);
	
	if (EventData.Target)
	{
		const AActor* Target = EventData.Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);

		if (const IUIHandler* Handler = Cast<IUIHandler>(ConstPlacement))
		{
			const_cast<IUIHandler*>(Handler)->OnClosed();
		}
		
		ConstPlacement->Multicast_SetCollisionEnabled(true);

		FVector Point = ConstPlacement->GetEndInteractPoint();
		FRotator Rotation = ConstPlacement->GetEndInteractRotation();
	
		Player->SetActorLocation(Point);
		Player->GetController()->SetControlRotation(Rotation);
		Player->SetActorRotation(Rotation);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
