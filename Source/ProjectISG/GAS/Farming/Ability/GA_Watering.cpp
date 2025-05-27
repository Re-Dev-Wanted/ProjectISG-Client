#include "GA_Watering.h"

#include "ProjectISG/Contents/Building/Props/HoedField.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Animation/Manager/AnimMontageManager.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

void UGA_Watering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	if (!IsValid(Player))
	{
		return;
	}

	AT_WateringAnim = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		UAnimMontageManager::GetMontage(
			Player->GetPlayerState<AMainPlayerState>(),
			EAnimMontageKey::Farming_Watering),
		FGameplayTagContainer(),
		*TriggerEventData
	);

	AT_WateringAnim->Activate();
	BlockInputForMontage(true);
	AT_WateringAnim->OnCompleted.
	                 AddDynamic(this, &ThisClass::OnEndWateringAnim);
}

void UGA_Watering::EndAbility(const FGameplayAbilitySpecHandle Handle,
                              const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo
                              ActivationInfo,
                              bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_Watering::OnEndWateringAnim(FGameplayTag EventTag,
                                     FGameplayEventData EventData)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	if (EventData.Target)
	{
		Player->GetInteractionComponent()->Server_OnTouchResponse(Player);
	}

	BlockInputForMontage(false);
	LoggingToWatering();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true,
	           false);
}

void UGA_Watering::LoggingToWatering()
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("농장");
	LogParams.ActionType = ELoggingActionType::FARMING;
	LogParams.ActionName = ELoggingActionName::water_crop;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingDataWithScreenshot(LogParams, true);
}
