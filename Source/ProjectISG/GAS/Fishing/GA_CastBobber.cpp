// Fill out your copyright notice in the Description page of Project Settings.

#include "GA_CastBobber.h"

#include "ProjectISG/Contents/Fishing/Props/FishingRod.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/Component/PlayerHandSlotComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Modal/Fishing/UI/UIC_FishingUI.h"
#include "ProjectISG/Systems/Animation/Manager/LevelSequenceManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "Task/AT_StartFishingCinematic.h"

class AMainPlayerState;

void UGA_CastBobber::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                     const FGameplayAbilityActorInfo* ActorInfo,
                                     const FGameplayAbilityActivationInfo
                                     ActivationInfo,
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

	const TObjectPtr<ABaseActor> Equipment = Player->GetHandSlotComponent()->
		GetHeldItem();

	if (!Equipment)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	const FHitResult TargetTraceResult = Player->GetInteractionComponent()->
	                                             GetTargetTraceResult();

	if (AFishingRod* FishingRod = Cast<AFishingRod>(Equipment))
	{

		if (Player->IsLocallyControlled())
		{
			AMainPlayerController* PC = Player->GetController<
				AMainPlayerController>();

			PC->SetIgnoreLookInput(true);
			PC->SetIgnoreMoveInput(true);

			Player->GetInteractionComponent()->SetIsInteractive(false);
		}

		AT_StartFishingCinematic = UAT_StartFishingCinematic::InitialEvent(
			this, ULevelSequenceManager::GetLevelSequence(
				Player->GetPlayerState<AMainPlayerState>(),
				ELevelSequenceKey::FishingStart));

		AT_StartFishingCinematic->OnStartFishingCinematicEndNotified.AddDynamic(
			this, &UGA_CastBobber::OnEndCinematic);
		AT_StartFishingCinematic->ReadyForActivation();

		Logging();

		return;
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_CastBobber::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo
                                ActivationInfo, bool bReplicateEndAbility,
                                bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_CastBobber::OnEndCinematic()
{
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>
		(CurrentActorInfo->AvatarActor.Get());

	if (!Player)
	{
		return;
	}

	const TObjectPtr<ABaseActor> Equipment = Player->GetHandSlotComponent()->
		GetHeldItem();

	if (!Equipment)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
			   false);
		return;
	}

	const FHitResult TargetTraceResult = Player->GetInteractionComponent()->
												 GetTargetTraceResult();

	if (AFishingRod* FishingRod = Cast<AFishingRod>(Equipment))
	{
		FishingRod->StartCasting(CurrentActorInfo->AvatarActor.Get(),
						 TargetTraceResult.ImpactPoint);
	}

	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(
		Player->GetController());

	if (PlayerController && Player->IsLocallyControlled())
	{
		PlayerController->PushUI(EUIName::Modal_FishingUI);

		UUIC_FishingUI* ModalUIController =
			Cast<UUIC_FishingUI>(PlayerController->GetUIManageComponent
				()->ControllerInstances[EUIName::Modal_FishingUI]);

		ModalUIController->SetUI(false, TEXT("RM"), TEXT("회수하기"));
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
	           false);
}

void UGA_CastBobber::Logging()
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("연못");
	LogParams.ActionType = ELoggingActionType::FISHING;
	LogParams.ActionName = ELoggingActionName::cast_bait;
	LogParams.Detail = TEXT("낚시를 시작했다.");

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingDataWithScreenshot(LogParams);
}
