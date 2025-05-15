#include "MainPlayerController.h"

#include "ProjectISG/Core/GameMode/MainGameState.h"
#include "ProjectISG/Core/UI/UIEnum.h"
#include "ProjectISG/Core/PlayerState/MainPlayerState.h"
#include "ProjectISG/Core/UI/Base/MVC/BaseUIController.h"
#include "ProjectISG/Core/UI/Gameplay/MainHUD/UI/UIC_MainHUD.h"
#include "ProjectISG/Core/UI/Base/Components/UIManageComponent.h"
#include "ProjectISG/Core/UI/Popup/Inventory/UI/UIC_InventoryUI.h"
#include "ProjectISG/Systems/QuestStory/Component/QuestManageComponent.h"

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
		GetPlayerState<AMainPlayerState>()->InitializeData();
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
	}
}

void AMainPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	GetPlayerState<AMainPlayerState>()->InitializeData();

	if (IsLocalController())
	{
		UIManageComponent->PushWidget(EUIName::Gameplay_MainHUD);
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

void AMainPlayerController::StartQuestToPlayer(const FString& QuestId)
{
	if (!IsLocalController())
	{
		return;
	}

	QuestManageComponent->StartQuest(QuestId);
}

void AMainPlayerController::StartScene(const FString& SceneId) const
{
	QuestManageComponent->StartScene(SceneId);
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

void AMainPlayerController::Client_ResetWidgetAndPushTimeAlert_Implementation()
{
	UIManageComponent->ResetWidget();
	UIManageComponent->PushWidget(EUIName::Modal_TimeAlert);
}
