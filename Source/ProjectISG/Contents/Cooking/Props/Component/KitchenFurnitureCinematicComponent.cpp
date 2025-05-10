#include "KitchenFurnitureCinematicComponent.h"

#include "Camera/CameraComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "ProjectISG/Contents/Cooking/Props/KitchenFurniture.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

UKitchenFurnitureCinematicComponent::UKitchenFurnitureCinematicComponent()
{
	LevelSequenceActor = nullptr;
	SelectedTarget = nullptr;

	SetIsReplicatedByDefault(true);
}

void UKitchenFurnitureCinematicComponent::BeginPlay()
{
	Super::BeginPlay();

	// 여기서 안되면 에러 즉, 해당 컴포넌트는 반드시
	// Kitchen Furniture에 종속성을 가진다.
	Kitchen = Cast<AKitchenFurniture>(GetOwner());
}

void UKitchenFurnitureCinematicComponent::StartCookingMode(
	AMainPlayerCharacter* Target, const bool IsCaster)
{
	SelectedTarget = Target;

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), StartCookingCinematic, PlaybackSettings
		, LevelSequenceActor);

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), Target);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")), Kitchen);

	LevelSequenceActor->SetActorTransform(Target->GetTransform());

	// 자기 자신인 경우에 대한 설정 처리
	if (IsCaster)
	{
		LevelSequencePlayer->OnFinished.AddDynamic(
			this, &ThisClass::OnFinishStartCooking);

		AMainPlayerController* PC = Target->GetController<
			AMainPlayerController>();
		Target->GetCharacterMovement()->DisableMovement();

		PC->PopUI();
		PC->SetIgnoreLookInput(true);
		PC->SetViewTargetWithBlend(Kitchen, 0.5f);
		Kitchen->GetKitchenCameraComponent()->Activate();
	}

	LevelSequencePlayer->Play();
}

void UKitchenFurnitureCinematicComponent::EndCookingMode(
	AMainPlayerCharacter* Target, const bool IsCaster)
{
	SelectedTarget = Target;

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), EndCookingCinematic, PlaybackSettings, LevelSequenceActor);

	if (IsCaster)
	{
		LevelSequencePlayer->OnFinished.AddDynamic(
			this, &ThisClass::OnFinishEndCooking);
	}

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player"))
										, Kitchen->GetUsingCharacter());
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")), Kitchen);

	LevelSequenceActor->SetActorTransform(
		Kitchen->GetUsingCharacter()->GetTransform());

	LevelSequencePlayer->Play();
}

void UKitchenFurnitureCinematicComponent::OnFinishStartCooking()
{
	AMainPlayerController* PC = SelectedTarget->GetController<
		AMainPlayerController>();
	PC->PushUI(EUIName::Popup_CookingRecipeUI);
	LoggingStartCooking();
}

void UKitchenFurnitureCinematicComponent::OnFinishEndCooking()
{
	AMainPlayerController* PC = SelectedTarget->GetController<
		AMainPlayerController>();

	// UI 원래대로 돌려두기
	PC->PushUI(EUIName::Gameplay_MainHUD);
	PC->SetIgnoreLookInput(false);
	PC->SetViewTargetWithBlend(SelectedTarget, 0.5f);

	// 카메라 시점 조정하기 및 움직임 처리
	SelectedTarget->bUseControllerRotationYaw = true;
	SelectedTarget->GetInteractionComponent()->SetIsInteractive(true);
	SelectedTarget->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	SelectedTarget->GetCameraComponent()->Activate();

	// 상호작용 관련 롤백 처리
	SelectedTarget->GetInteractionComponent()->SetIsInteractive(true);
	SelectedTarget->GetInteractionComponent()->SetSelectedInteractiveActor(
		nullptr);

	LoggingEndCooking();

	Kitchen->SetUsingOwner(nullptr);
}

void UKitchenFurnitureCinematicComponent::LoggingStartCooking() const
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("요리장");
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::start_cooking;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
				LoggingData(LogParams);
}

void UKitchenFurnitureCinematicComponent::LoggingEndCooking() const
{
	FDiaryLogParams LogParams;
	LogParams.Location = TEXT("요리장");
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::finish_cooking;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
				LoggingData(LogParams);
}
