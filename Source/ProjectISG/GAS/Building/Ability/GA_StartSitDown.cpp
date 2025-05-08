#include "GA_StartSitDown.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Kismet/KismetSystemLibrary.h"
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
		const AActor* Target = TriggerEventData->Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);
		ConstPlacement->SetCollisionEnabled(false);

		APlacement* Placement = const_cast<APlacement*>(ConstPlacement);

		FVector Point = Placement->GetStartInteractPoint();
		FVector PlayerLocation = Player->GetActorLocation();

		FVector StartLocation = FVector(Point.X, Point.Y, PlayerLocation.Z);
		
		Player->SetActorLocation(StartLocation, false);
		
		FRotator YawOnly(0.f, 180.f, 0.f);
		Player->SetActorRotation(YawOnly);
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
	
	Player->GetController()->SetIgnoreLookInput(true);
	Player->GetController()->SetIgnoreMoveInput(true);

	Player->GetInteractionComponent()->SetIsInteractive(false);

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
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("NotifyMontage"));
		const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>
		(CurrentActorInfo->AvatarActor.Get());

		if (!Player)
		{
			return;
		}
		
		if (CurrentEventData.Target)
		{
			const AActor* Target = CurrentEventData.Target.Get();
			const APlacement* ConstPlacement = Cast<APlacement>(Target);
			ConstPlacement->SetCollisionEnabled(true);
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

		if (PlayerController)
		{
			PlayerController->PushUI(EUIName::Modal_ExitInteractUI);

			UUIC_ExitInteractUI* ModalUIController = 
			Cast<UUIC_ExitInteractUI>(PlayerController->GetUIManageComponent
			()->ControllerInstances[EUIName::Modal_ExitInteractUI]);

			ModalUIController->SetUI(TEXT("X"), TEXT("나가기"));
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

