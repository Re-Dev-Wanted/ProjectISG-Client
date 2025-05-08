// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SitDown.h"

#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"

void UAnimNotify_SitDown::Notify(USkeletalMeshComponent* MeshComp,
                                 UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
	{
		return;
	}

	const AActor* Owner = MeshComp->GetOwner();

	if (!Owner)
	{
		return;
	}

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Owner);

	if (!Player)
	{
		return;
	}

	FGameplayEventData EventData;
	EventData.EventTag = ISGGameplayTags::Building_Active_NotifySitDown;
	EventData.Instigator = Player;

	Player->GetAbilitySystemComponent()->HandleGameplayEvent
	(ISGGameplayTags::Building_Active_NotifySitDown, &EventData);
}
