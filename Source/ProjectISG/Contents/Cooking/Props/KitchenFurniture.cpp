#include "KitchenFurniture.h"

#include "Camera/CameraComponent.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectISG/Contents/Cooking/CookingStruct.h"
#include "ProjectISG/Core/Character/Player/MainPlayerCharacter.h"
#include "ProjectISG/Core/Controller/MainPlayerController.h"
#include "ProjectISG/Core/Character/Player/Component/InteractionComponent.h"
#include "ProjectISG/Systems/Logging/LoggingStruct.h"
#include "ProjectISG/Systems/Logging/LoggingSubSystem.h"

AKitchenFurniture::AKitchenFurniture()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	KitchenStandPosition = CreateDefaultSubobject<USceneComponent>(
		"Kitchen Stand Position");
	KitchenStandPosition->SetupAttachment(Mesh);

	KitchenCameraComponent = CreateDefaultSubobject<UCameraComponent>(
		"Kitchen Camera Component");
	KitchenCameraComponent->SetupAttachment(Mesh);

	FryPanMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Fry Pan Mesh");
	FryPanMesh->SetupAttachment(GetRootComponent());

	ScoopMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Scoop Mesh");
	ScoopMesh->SetupAttachment(GetRootComponent());

	WokMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		"Wok Mesh");
	WokMesh->SetupAttachment(GetRootComponent());
}

bool AKitchenFurniture::GetCanInteractive() const
{
	return UsingCharacter == nullptr;
}

FString AKitchenFurniture::GetInteractiveDisplayText() const
{
	return TEXT("요리하기");
}

void AKitchenFurniture::OnInteractive(AActor* Causer)
{
	IInteractionInterface::OnInteractive(Causer);

	if (AMainPlayerCharacter* Player = Cast<AMainPlayerCharacter>(Causer))
	{
		UsingCharacter = Player;

		Player->bUseControllerRotationYaw = false;
		Player->SetActorTransform(
			KitchenStandPosition->GetComponentTransform());

		if (!Player->HasAuthority())
		{
			Player->Server_SetActorTransformReplicated(
				KitchenStandPosition->GetComponentTransform());
		}

		Player->GetInteractionComponent()->SetIsInteractive(false);
		Player->GetInteractionComponent()->SetSelectedInteractiveActor(this);

		// TODO: RPC 함수 호출로 모든 곳에서 동일한
		// 동작을 실행시켜야 한다.
		StartCookingMode();
	}
}

void AKitchenFurniture::StartCookingMode()
{
	AMainPlayerController* PC = UsingCharacter->GetController<
		AMainPlayerController>();

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), StartCookingCinematic,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(
		this, &ThisClass::OnFinishStartCooking);

	UsingCharacter->GetCharacterMovement()->DisableMovement();

	PC->SetIgnoreLookInput(true);
	PC->SetViewTargetWithBlend(this, 0.5f);

	GetKitchenCameraComponent()->Activate();

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), UsingCharacter);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")), this);

	LevelSequenceActor->SetActorTransform(UsingCharacter->GetTransform());

	LevelSequencePlayer->Play();
}

void AKitchenFurniture::EndCookingMode()
{
	AMainPlayerController* PC = UsingCharacter->GetController<
		AMainPlayerController>();

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = true;

	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), EndCookingCinematic,
		PlaybackSettings, LevelSequenceActor);

	LevelSequencePlayer->OnFinished.AddDynamic(
		this, &ThisClass::OnFinishEndCooking);

	UsingCharacter->GetCharacterMovement()->DisableMovement();

	PC->SetIgnoreLookInput(true);
	PC->SetViewTargetWithBlend(this, 0.5f);

	GetKitchenCameraComponent()->Activate();

	LevelSequenceActor->AddBindingByTag(FName(TEXT("Player")), UsingCharacter);
	LevelSequenceActor->AddBindingByTag(FName(TEXT("Interactive")), this);

	LevelSequenceActor->SetActorTransform(UsingCharacter->GetTransform());

	LevelSequencePlayer->Play();
}

void AKitchenFurniture::EquipCookingToolToAct(
	const FEquipCookingToolToActParams& Params)
{
	switch (Params.CookingTool)
	{
	case ECookingTool::FryingPan:
		{
			FryPanMesh->SetVisibility(true);
			FryPanMesh->AttachToComponent(Params.AttachPoint,
			                              FAttachmentTransformRules::SnapToTargetIncludingScale,
			                              TEXT("hand_r"));
			break;
		}
	case ECookingTool::Wok:
		{
			ScoopMesh->SetVisibility(true);
			ScoopMesh->AttachToComponent(Params.AttachPoint,
			                             FAttachmentTransformRules::SnapToTargetIncludingScale,
			                             TEXT("scoop_socket"));
			ScoopMesh->SetRelativeRotation(FRotator::ZeroRotator);

			WokMesh->SetVisibility(true);
			WokMesh->AttachToComponent(Params.AttachPoint,
			                           FAttachmentTransformRules::SnapToTargetIncludingScale,
			                           TEXT("wok_socket"));
			WokMesh->SetRelativeRotation(FRotator::ZeroRotator);

			break;
		}
	default:
		{
			break;
		}
	}
}

void AKitchenFurniture::UnEquipCookingToolToAct()
{
	FryPanMesh->SetVisibility(false);
	FryPanMesh->AttachToComponent(GetRootComponent(),
	                              FAttachmentTransformRules::SnapToTargetIncludingScale);
	FryPanMesh->SetRelativeLocation(FVector::ZeroVector);

	ScoopMesh->SetVisibility(false);
	ScoopMesh->AttachToComponent(GetRootComponent(),
	                             FAttachmentTransformRules::SnapToTargetIncludingScale);
	ScoopMesh->SetRelativeLocation(FVector::ZeroVector);

	WokMesh->SetVisibility(false);
	WokMesh->AttachToComponent(GetRootComponent(),
	                           FAttachmentTransformRules::SnapToTargetIncludingScale);
	WokMesh->SetRelativeLocation(FVector::ZeroVector);
}

void AKitchenFurniture::OnFinishStartCooking()
{
	AMainPlayerController* PC = UsingCharacter->GetController<
		AMainPlayerController>();
	PC->PushUI(EUIName::Popup_CookingRecipeUI);
}

void AKitchenFurniture::OnFinishEndCooking()
{
	AMainPlayerController* PC = UsingCharacter->GetController<
		AMainPlayerController>();
	PC->PushUI(EUIName::Gameplay_MainHUD);
	PC->SetIgnoreLookInput(false);
	PC->SetViewTargetWithBlend(UsingCharacter, 0.5f);

	UsingCharacter->bUseControllerRotationYaw = true;
	UsingCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	UsingCharacter->GetCameraComponent()->Activate();

	UsingCharacter->GetInteractionComponent()->SetIsInteractive(true);
	UsingCharacter->GetInteractionComponent()->
	                SetSelectedInteractiveActor(nullptr);

	UsingCharacter = nullptr;
}

void AKitchenFurniture::LoggingStartCooking()
{
	FDiaryLogParams LogParams;
	LogParams.Location = "요리장";
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::start_cooking;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}

void AKitchenFurniture::LoggingEndCooking()
{
	FDiaryLogParams LogParams;
	LogParams.Location = "요리장";
	LogParams.ActionType = ELoggingActionType::COOKING;
	LogParams.ActionName = ELoggingActionName::finish_cooking;

	GetWorld()->GetGameInstance()->GetSubsystem<ULoggingSubSystem>()->
	            LoggingData(LogParams);
}
