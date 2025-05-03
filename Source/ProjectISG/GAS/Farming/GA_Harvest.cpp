// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Harvest.h"

#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Systems/Logging/LoggingEnum.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"
#include "ProjectISG/Utils/EnumUtil.h"


void UGA_Harvest::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo
                                  ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		ActorInfo->AvatarActor.Get());

	UE_LOG(LogTemp, Warning, TEXT("수확하기, %s"),
	       *FEnumUtil::GetClassEnumKeyAsString(Player->GetLocalRole()));
	LoggingToHarvest();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Harvest::EndAbility(const FGameplayAbilitySpecHandle Handle,
                             const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo
                             ActivationInfo,
                             bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,
	                  bWasCancelled);
}

void UGA_Harvest::LoggingToHarvest()
{
	FDiaryLogParams LogParams;
	LogParams.Location = "경작지";
	LogParams.ActionType = ELoggingActionType::FARMING;
	LogParams.ActionName = ELoggingActionName::harvest_crop;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
				LoggingData(LogParams);

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->Flush();
}
