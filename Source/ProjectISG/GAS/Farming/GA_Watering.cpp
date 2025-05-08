// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Watering.h"

#include "ProjectISG/Contents/Building/Props/HoedField.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Utils/EnumUtil.h"

void UGA_Watering::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                   const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo
                                   ActivationInfo,
                                   const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());
	if (!IsValid(Player))
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("물주기, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));

	AT_WateringAnim = UPlayMontageWithEvent::InitialEvent
	(
		this,
		NAME_None,
		Player->GetWateringMontage(),
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
	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		CurrentActorInfo->AvatarActor.Get());

	if (EventData.Target)
	{
		const AActor* Target = EventData.Target.Get();
		const AHoedField* ConstField = Cast<AHoedField>(Target);

		AHoedField* HoedField = const_cast<AHoedField*>(ConstField);
		HoedField->SetWet(true);
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

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->LoggingData(LogParams);

}
