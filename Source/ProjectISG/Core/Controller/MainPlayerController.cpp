﻿#include "MainPlayerController.h"

#include "ProjectISG/Core/ISGGameInstance.h"
#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Loading/UIC_Loading.h"
#include "ProjectISG/Core/UI/Loading/UIV_Loading.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Core/UI/Popup/SceneList/UI/CutSceneList/UIC_SceneListUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

class UUIC_Loading;
class UISGGameInstance;

AMainPlayerController::AMainPlayerController()
{
	UIManageComponent = CreateDefaultSubobject<UUIManageComponent>(
		"UI Manage Component");
	QuestManageComponent = CreateDefaultSubobject<UQuestManageComponent>(
		"Quest Manage Component");
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// ConsoleCommand(TEXT("showdebug abilitysystem"));

	if (IsLocalController())
	{
		UIManageComponent->Initialize();
	}

	if (IsLocalController() && HasAuthority())
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
		StartScene(FString::Printf(TEXT("Scene_7")));
		UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
			GetUIManageComponent()->ControllerInstances[
				EUIName::Popup_SceneListUI]);
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MainSceneEnd);
	}
}

void AMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (IsLocalController())
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
		StartScene(FString::Printf(TEXT("Scene_7")));
		UUIC_SceneListUI* SceneListUIController = Cast<UUIC_SceneListUI>(
			GetUIManageComponent()->ControllerInstances[
				EUIName::Popup_SceneListUI]);
		SceneListUIController->OnSceneListEndNotified.BindUObject(
			this, &ThisClass::MainSceneEnd);
	}
}

void AMainPlayerController::MainSceneEnd()
{
	if (HasAuthority())
	{
		StartQuest(FString::Printf(TEXT("Story_000")));
	}
}

void AMainPlayerController::StartQuest(const FString& QuestId)
{
	// 서버면 GameState를 통해서 모든 PlayerController에 Quest 수행을 명령한다.
	if (HasAuthority())
	{
		GetWorld()->GetGameState<AMainGameState>()->StartWorldQuest(QuestId);
	}
	else
	{
		// 서버가 아니면 서버에서 호출하도록 RPC 함수를 호출한다.
		Server_StartQuest(QuestId);
	}
}

void AMainPlayerController::EndQuest()
{
	if (!HasAuthority())
	{
		return;
	}

	GetWorld()->GetGameState<AMainGameState>()->EndWorldQuest();
}

void AMainPlayerController::StartQuestToPlayer(const FString& QuestId)
{
	if (!IsLocalController())
	{
		return;
	}

	QuestManageComponent->StartQuest(QuestId);
}

void AMainPlayerController::StartScene(const FString SceneId) const
{
	UE_LOG(LogTemp, Warning, TEXT("StartScene"));
	QuestManageComponent->StartScene(SceneId);
}

void AMainPlayerController::ShowLoadingUIAndCreateSession(bool bIsServerTravel)
{
	PopUI();
	PushUI(EUIName::Loading_LoadingUI);

	const UUIC_Loading* LoadingController = Cast<UUIC_Loading>(
		UIManageComponent->ControllerInstances[EUIName::Loading_LoadingUI]);
	UUIV_Loading* LoadingView = Cast<
		UUIV_Loading>(LoadingController->GetView());
	LoadingView->SetCreateSession(true);

	UISGGameInstance* GameInstance = Cast<UISGGameInstance>(
		GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		GameInstance->SetIsServerTravel(bIsServerTravel);
	}
}

TObjectPtr<UUIC_MainHUD> AMainPlayerController::GetMainHUD() const
{
	// 컨트롤러가 없는 경우에만 만들기
	if (!UIManageComponent->ControllerInstances.FindRef(
		EUIName::Gameplay_MainHUD) && UIManageComponent->HasViewUI(
		EUIName::Gameplay_MainHUD))
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
	}

	return Cast<UUIC_MainHUD>(
		UIManageComponent->ControllerInstances.FindRef(
			EUIName::Gameplay_MainHUD));
}

TObjectPtr<UUIC_InventoryUI> AMainPlayerController::GetInventoryUI() const
{
	// 컨트롤러가 없는 경우에만 만들기
	if (!UIManageComponent->ControllerInstances.FindRef(
		EUIName::Popup_InventoryUI) && UIManageComponent->HasViewUI(
		EUIName::Popup_InventoryUI))
	{
		UIManageComponent->PushWidget(EUIName::Popup_InventoryUI);
	}

	return Cast<UUIC_InventoryUI>(
		UIManageComponent->ControllerInstances.FindRef(
			EUIName::Popup_InventoryUI));
}

void AMainPlayerController::PushUI(const EUIName UIName)
{
	UIManageComponent->PushWidget(UIName);
}

void AMainPlayerController::PopUI()
{
	UIManageComponent->PopWidget();
}

void AMainPlayerController::Alert(const EAlertType AlertType
								, const FString& Message, const float Time)
{
	if (UIManageComponent->GetTopStackUI() != EUIName::Gameplay_MainHUD)
	{
		return;
	}

	GetMainHUD()->AlertToMainHUD(AlertType, Message, Time);
}

void AMainPlayerController::OpenInventory()
{
	if (!IsLocalController())
	{
		return;
	}

	UIManageComponent->PushWidget(EUIName::Popup_InventoryUI);
}

void AMainPlayerController::Server_StartQuest_Implementation(
	const FString& QuestId)
{
	GetWorld()->GetGameState<AMainGameState>()->StartWorldQuest(QuestId);
}

void AMainPlayerController::Client_StartQuestToPlayer_Implementation(
	const FString& QuestId)
{
	StartQuestToPlayer(QuestId);
}

void AMainPlayerController::Client_EndQuestToPlayer_Implementation()
{
	QuestManageComponent->EndQuest(false);
}

void
AMainPlayerController::Client_ResetWidgetAndPushContentsTimeAlert_Implementation()
{
	UIManageComponent->ResetWidget();
	Alert(EAlertType::Error, TEXT("저녁 8시 이후에는 컨텐츠가 제한됩니다!!!"), 6.0f);
}

void AMainPlayerController::Client_PushForceSleepTimeAlert_Implementation()
{
	UIManageComponent->ResetWidget();
	Alert(EAlertType::Error, TEXT("저녁 11시에 강제로 수면에 들어갑니다!!!"), 6.0f);
}

void AMainPlayerController::Server_SetOwnerActor_Implementation(
	AActor* ResponseActor)
{
	ResponseActor->SetOwner(this);
}
