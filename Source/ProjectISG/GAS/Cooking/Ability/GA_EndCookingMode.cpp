#include "GA_EndCookingMode.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "Task/AT_EndCookingModeCinematic.h"

void UGA_EndCookingMode::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	Player->GetController<AMainPlayerController>()->PopUI();

	AT_EndCookingModeCinematic = UAT_EndCookingModeCinematic::InitialEvent(
		this, EndCookingCinematic);

	AT_EndCookingModeCinematic->OnEndCookingModeCinematicEndNotified.
	                            AddDynamic(this, &ThisClass::OnEndCinematic);
	AT_EndCookingModeCinematic->ReadyForActivation();
}

void UGA_EndCookingMode::OnEndCinematic()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	AMainPlayerController* PC = Player->GetController<
		AMainPlayerController>();

	PC->PushUI(EUIName::Gameplay_MainHUD);
	PC->SetIgnoreLookInput(false);
	PC->SetViewTargetWithBlend(Player, 0.5f);

	PC->GetUIManageComponent()->ResetWidget();

	UnlockPlayer();

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
	           false, false);
}

void UGA_EndCookingMode::UnlockPlayer()
{
	AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(
		GetAvatarActorFromActorInfo());

	AKitchenFurniture* KitchenFurniture = Cast<AKitchenFurniture>(
		Player->GetInteractionComponent()->GetTargetTraceResult().GetActor());

	KitchenFurniture->UnlockPlayer();

	Player->bUseControllerRotationYaw = true;
	Player->GetInteractionComponent()->SetIsInteractive(true);
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Player->GetCameraComponent()->Activate();
}
