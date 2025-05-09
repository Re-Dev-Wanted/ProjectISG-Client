#include "GA_StartSitDown.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Modal/Interactive/UI/UIC_ExitInteractUI.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"

void UGA_StartSitDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get());
	
	if (!Player)
	{
		return;
	}

	if (TriggerEventData->Target)
	{
		Player->bUseControllerRotationYaw = false;
		Player->GetCharacterMovement()->bOrientRotationToMovement = true;

		Player->GetController()->SetIgnoreMoveInput(false);
		Player->GetInteractionComponent()->SetIsInteractive(false);
		
		const AActor* Target = TriggerEventData->Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);
		ConstPlacement->SetCollisionEnabled(false);

		APlacement* Placement = const_cast<APlacement*>(ConstPlacement);
		
		FVector Point = Placement->GetStartInteractPoint();
		FRotator Rotation = Placement->GetStartInteractRotation();
		
		FVector PlayerLocation = Player->GetActorLocation();
		
		Player->SetActorLocation(Point);
		Player->GetController()->SetControlRotation(Rotation);
		Player->SetActorRotation(Rotation);
	}

	AT_StartMontageEvent = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		StartSitDownMontage,
		FGameplayTagContainer(ISGGameplayTags::Building_Active_NotifySitDown),
		FGameplayEventData()
	);
	
	AT_StartMontageEvent->EventReceived.AddDynamic(this, 
	&UGA_StartSitDown::NotifyMontage);

	AT_StartMontageEvent->ReadyForActivation();
}

void UGA_StartSitDown::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}

void UGA_StartSitDown::NotifyMontage(FGameplayTag EventTag,
	FGameplayEventData EventData)
{
	if (EventTag == ISGGameplayTags::Building_Active_NotifySitDown)
	{
		const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>
		(CurrentActorInfo->AvatarActor.Get());

		if (!Player)
		{
			return;
		}

		UAbilityTask_WaitGameplayEvent* WaitEvent = 
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent
		(
			this,
			ISGGameplayTags::Interactive_Active_Exit,
			nullptr,
			true,
			false
		);

		WaitEvent->EventReceived.AddDynamic(this, &UGA_StartSitDown::EndMontage);
		WaitEvent->ReadyForActivation();

		AMainPlayerController* PlayerController = Cast<AMainPlayerController>( 
		Player->GetController());

		if (PlayerController && Player->IsLocallyControlled())
		{
			PlayerController->PushUI(EUIName::Modal_ExitInteractUI);

			UUIC_ExitInteractUI* ModalUIController = 
			Cast<UUIC_ExitInteractUI>(PlayerController->GetUIManageComponent
			()->ControllerInstances[EUIName::Modal_ExitInteractUI]);

			ModalUIController->SetUI(TEXT("F"), TEXT("나가기"));
		}
		
	}
}


void UGA_StartSitDown::EndMontage(FGameplayEventData Payload)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Building_Active_EndSitDown);
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ActivateTag);
}

