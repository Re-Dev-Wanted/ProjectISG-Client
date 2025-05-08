#include "GA_StartSitDown.h"

#include "AbilitySystemComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/GAS/Common/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectISG/GAS/Common/Tag/ISGGameplayTag.h"
#include "ProjectISG/Systems/Grid/Actors/Placement.h"

void UGA_StartSitDown::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(ActorInfo->AvatarActor.Get());
	
	if (!IsValid(Player))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (TriggerEventData->Target)
	{
		const AActor* Target = TriggerEventData->Target.Get();
		const APlacement* ConstPlacement = Cast<APlacement>(Target);

		APlacement* Placement = const_cast<APlacement*>(ConstPlacement);

		FVector Point = Placement->GetInteractStartPoint()->GetComponentLocation();
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
		FGameplayTagContainer(),
		FGameplayEventData()
	);

	AT_StartMontageEvent->OnCompleted.AddDynamic(this, &UGA_StartSitDown::EndMontage);
	
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

void UGA_StartSitDown::EndMontage(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	
	FGameplayTagContainer ActivateTag;
	ActivateTag.AddTag(ISGGameplayTags::Building_Active_EndSitDown);
	Player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ActivateTag);
}
